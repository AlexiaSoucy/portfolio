# Author: Alexia Soucy, Concordia University, Fall 2019, COMP 472 with Nora Houari
from __future__ import division
from collections import Counter
import numpy as np
from codecs import open
import sys
import os

# Setting up various variables for operations
smoothing = 4.1
trainingFile = ""
evalFile = ""
outputFile = 'classifications.txt'

# Select training file
trainingFile = input("Training file: ")
while not(os.path.isfile(trainingFile)):
	print("Invalid training file. Please input an existing file.")
	trainingFile = input("Training file: ")

# Determine if user wants to use a secondary eval file
print("Use custom eval file? (if no, the last 20% of the training file will be used to demo eval)")
customEval = input("Use custom eval file (y/n): ")
while customEval != "y" and customEval != "n":
	print("Invalid input. Please input y or n.")
	customEval = input("Use custom eval file (y/n): ")

# Select eval file
if customEval == "y":
	evalFile = input("Eval file: ")
	while not(os.path.isfile(evalFile)):
		print("Invalid eval file. Please input an existing file.")
		evalFile = input("Eval file: ")
else:
	evalFile = trainingFile

# Extracts reviews from file as a list of documents (split into words) and a list of labels
# SAMPLE CODE PROVIDED BY IN PROJECT SPECS
def read_documents(doc_file):
	docs = []
	labels = []

	with open(doc_file, encoding='utf-8') as f:
		for line in f:
			words = line.strip().split()
			docs.append(words[3:]) # Review starts at the 4th word
			labels.append(words[1]) # Label is the 2nd word

	return docs, labels

# Employs Naive-Bayes training algorithm on two lists of documents and labels with add-1 smoothing
# RETURN: Dictionary of labels with label probability and nested dictionary of individual word probability given the label
def train_nb(documents, labels):
	# Get a dictionary of unique labels
	labelDict = {}
	for label in labels:
		# If the label does not exist...
		if label not in labelDict:
			# For each entry:
			#	[0]: Label probability
			#	[1]: Dictionary of individual word probabilities (initially individual word counts)
			labelDict[label] = [0,{}]

	# Count vocabulary size for smoothing
	totalFreqs = Counter()
	for doc in documents:
		totalFreqs.update(doc)
	vocabSize = len(totalFreqs)

	# Count each word for each label
	for label in labelDict:
		wordFreqs = Counter()
		# For each document...
		for doc in documents:
			# If the labels match
			if labels[documents.index(doc)] == label:
				# Count words
				wordFreqs.update(doc)

		# Add counts to label subdict, even for words that aren't in this label (add smoothing)
		wordsum = sum(wordFreqs.values())
		for word in totalFreqs:
			labelDict[label][1][word] = wordFreqs[word] + smoothing

		# Overwrite each word count for its probability given the label
		for word in labelDict[label][1]:
			# Word's probability given label is the word's count in this label plus smoothing divided by the label's total word count plus vocab size
			labelDict[label][1][word] = labelDict[label][1][word] / (wordsum + vocabSize)

	# Count each label
	labelFreqs = Counter()
	labelFreqs.update(labels)
	docsum = sum(labelFreqs.values())

	# Count the probability of each label
	for label in labelFreqs:
		# Get label's probability
		labelDict[label][0] = labelFreqs[label] / docsum

	return labelDict

# Scores a document based on Naive-Bayes algorithm
# RETURN: The log probability of the document's words appearing in the label, based on labelDict
def score_doc_label(document, label, labelDict):
	# Get the label's log probability
	prob = np.log(labelDict[label][0])

	# Add the individual word log probabilities for words that are in the vocabulary
	for word in document:
		if word in labelDict[label][1]:
			prob += np.log(labelDict[label][1][word])

	return prob

# Classifies a new document based on Naive-Bayes algorithm
# RETURN: The label assigned based on Naive-Bayes
def classify_nb(document, labelDict):

	# Initialize classification
	bestLabel = ""
	bestProb = -float('inf')

	# Check each label's probability for the best one
	for label in labelDict:
		prob = score_doc_label(document, label, labelDict)

		if prob > bestProb:
			bestProb = prob
			bestLabel = label

	return bestLabel

# Classifies a list of documents per Naive-Bayes
# RETURN: A list of classifications for the documents
def classify_documents(docs, labelDict):
	classifications = []

	# Classify each doc and add its class to the list
	for doc in docs:
		classifications.append(classify_nb(doc, labelDict))

	return classifications

# Compares a list of true labels and guessed labels to find the guessed labels' accuracy
# RETURN: accuracy in percentage
def accuracy(true_labels, guessed_labels, true_docs):
	totalCount = 0
	trueCount = 0

	# Compare labels
	for i in range(0, len(true_labels)):
		totalCount += 1
		if true_labels[i] == guessed_labels[i]:
			trueCount += 1

	return trueCount / totalCount * 100

# Allow for separate training and eval files
train_docs = []
train_labels = []
eval_docs = []
eval_labels = []

# If training and eval files are the same, split 80-20, otherwise assume the user has provided good files
if evalFile == trainingFile:
	# SAMPLE CODE PROVIDED IN PROJECT SPECS
	all_docs, all_labels = read_documents(trainingFile)

	# Use 80% of provided docs for training, 20% for eval
	split_point = int(0.80*len(all_docs))
	train_docs = all_docs[:split_point]
	train_labels = all_labels[:split_point]
	eval_docs = all_docs[split_point:]
	eval_labels = all_labels[split_point:]
	# SAMPLE CODE ENDS HERE
else:
	train_docs, train_labels = read_documents(trainingFile)
	eval_docs, eval_labels = read_documents(evalFile)


# Train the Naive-Bayes algorithm on the given data
print("TRAINING WITH " + trainingFile)
labelDict = train_nb(train_docs, train_labels)

# Evaluate remaining docs
print("EVALUATING " + evalFile)
guesses = classify_documents(eval_docs, labelDict)
acc = accuracy(eval_labels, guesses, eval_docs)

# Output to file
# NOTE: No specifications provided regarding output format in project handout.
print("WRITING TO " + outputFile)
out = open(outputFile, 'w')
out.write("ACCURACY: " + str(acc) + "%")
for i in range(0, len(eval_docs)):
	docText = ""
	for word in eval_docs[i]:
		docText += word + " "
	out.write("\n\nGUESS: " + guesses[i] + " ACTUAL: " + eval_labels[i] + " DOC: " + docText)
out.close()

# Print accuracy
print("ACCURACY: " + str(acc) + "%")

# ERROR ANALYSIS
#	GUESS: neg ACTUAL: pos
#		"i liked everything about this product except it is a little too slow at times. i always shoot with nikon raw format.
#		 it has hung a couple of times and i had to kill it after waiting for 10-15 minutes."
#
#		ANALYSIS: 	Although the review is positive, most of it concerns flaws in the product.
#					As such, a statistical analysis system such as Naive-Bayes will tend to 
#					classify it negatively if most reviews the system is trained on are entirely 
#					negative or entirely positive, since the language used here is determined to
#					be statistically unlikely for a positive review.
#
#	GUESS: neg ACTUAL: pos
#		"i was a little diappointed when i found out that i would have to buy more versions to get continued training on this
#		 subject."
#
#		ANALYSIS:	Similarly to the previous review, this one simply states a perceived flaw.
#					Words such as "disappointed" will skew results toward the negative.
#
#	GUESS: neg ACTUAL: pos
#		"i absolutely love this stuff. i would recommend this to any person that has frizz. it makes my hair feel so silky smoothe.
#		 i started using it about 2 months ago and cannot stand to go without it. just use a little because it can make your hair
#		 look oily if you use too much and don't put it close to the scalp either for the same reason. i tried the chi silk
#		 infusion because i thought they would be the same since they are both made by farouk but the chi is not nearly as good.
#		 i would give this 100 stars i could."
#
#		ANALYSIS:	The review is positive, but it compares the product to another lesser one,
#					adding negative vocabulary to the review. It also provides tips for how
#					to avoid misusing the product, which Naive-Bayes cannot detect with word-by-word
#					analysis.
#
#	GUESS: pos ACTUAL: neg
#		"i was so disappointed in this book. i don't find a closeness to god in the pages. instead it seems intellectual.
#		 it contains beautiful sounding prayers that are more like poetry than speaking with god face to face. even though
#		 it contains prayers from across the ages it seems to keep me distant from the personal god i seek. and though the
#		 author's prayers are included too, i feel the same about them. i expected more from richard j. foster"
#
#		ANALYSIS:	The review is meant to be negative, but much of its vocabulary is positive
#					since it concerns what the reviewer was looking for rather than what they
#					they got. Additionally, the reviewer considers the product's poetic and
#					intellectual characteristics to be negative, which does not match with the 
#					system's training.
#
#	GUESS: pos ACTUAL: neg
#		"not particularly musical; only 1 good track (blues for miles).."
#
#		ANALYSIS:	Phrases such as "not very good" will be overall evaluated as positives.
#					(although in this case 'musical' doesn't carry much weight, the word 
#					"particularly" undoes some of "not"'s negative character)
#					Similarly, "only" is read as negative, but the words that follow are
#					all positive or neutral. Particularly short reviews that list one positive
#					run the risk of being read as positive overall.
#
#	GUESS: neg ACTUAL: pos
#		"you will never look at these birds the same again - fansinating"
#
#		ANALYSIS:	Much of the language here is quite neutral, but words like "never," 
#					"same," and "again" read as negative. Also, the misspelled
#					"fascinating" means this word is not as impactful as it should be.
#
#	CONCLUSION:
#	As is to be expected, the Naive-Bayes algorithm's satistical analysis means it can be fooled by
#	entries that use many words that don't match the overall sentiment of the entry. This can either
#	take the form of honest reviews that list counterpoints to what they're getting at or reviews
#	that use compound expressions made up of multiple words opposite to their overall character.