# Author: Alexia Soucy, Concordia University, Fall 2019, COMP 472 with Nora Houari

# Import NumPy
import numpy as np
import time

# Game setup
moves = 30
depthLimit = 2
minToken = "■"
maxToken = "□"
board = np.array(
	[(" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," "),
	 (" "," "," "," "," "," "," "," "," "," "," "," ")])

class Player:
	def __init__(self, token, isHuman = True):
		self.tokens = 15
		self.token = token
		self.isHuman = isHuman

	def reset(self, isHuman = True):
		self.tokens = 15
		self.isHuman = isHuman

# Initialize players
p1 = Player("■", True)
p2 = Player("□", True)
players = (p1, p2)
currentTurn = 0

# Simple factorial
def factorial(val):
	if val <= 1:
		return 1
	else:
		return val * factorial(val-1)

class MinMaxNode:
	def __init__(self, isMin, boardVal = 0, alpha = -float('inf'), beta = float('inf'), parent = None, depth = 0, isMove = False, toRow = 0, toCol = 0, fromRow = 0, fromCol = 0):
		self.isMin = isMin
		self.boardVal = boardVal
		self.parent = parent
		self.depth = depth
		self.isMove = isMove
		self.toRow = toRow
		self.toCol = toCol
		self.fromRow = fromRow
		self.fromCol = fromCol
		self.alpha = alpha
		self.beta = beta

		if self.isMin:
			self.value = float('inf')
		else:
			self.value = -float('inf')
		self.children = []

	# Create children recursively and evaluate the value of leaves, then propagate it up with alpha-beta pruning
	def generate_children(self):
		# Check if placements/movements are possible for this player at this depth
		canPlace = True
		placeCount = 0
		canMove = True
		moveCount = 0

		# Go through ancestors sequentially
		ancestor = self.parent
		while ancestor is not None:
			# If the ancestor is a past move by the next player
			if ancestor.isMin != self.isMin:
				# Add move/place
				if ancestor.isMove:
					moveCount += 1
				else:
					placeCount += 1

			# Note the identity of the current player for preemptive pruning
			if ancestor.depth == 0:
				rootIsMin = ancestor.isMin

			ancestor = ancestor.parent

		# Check if placements are valid
		if players[currentTurn].tokens - placeCount <= 0:
			canPlace = False

		# Check if movements are valid
		if moves - moveCount <= 0:
			canMove = False

		# Determine which token is friendly
		friendlyToken = ""
		opponentToken = ""
		if self.isMin:
			friendlyToken = minToken
			opponentToken = maxToken
		else:
			friendlyToken = maxToken
			opponentToken = minToken

		pruned = False

		# Get own boardVal if not root
		if self.depth > 0:
			self.evaluate()

			# Preemptive pruning; doesn't explore a branch if it gives an opponent a winning move
			if self.boardVal == (-1) ** int(rootIsMin + 1) * float('inf'): # -inf if root max, inf if root min
				pruned = True
				self.value = self.boardVal

		# Add turn to board if not root
		if self.depth > 0:
			board[self.toRow][self.toCol] = opponentToken
			if self.isMove:
				board[self.fromRow][self.fromCol] = " "


		if canPlace or canMove:
			for off in range(0, 12):
				for row in range(max(0, self.toRow - off), min(10, self.toRow + off + 1)):
					for col in range(max(0, self.toCol - off), min(12, self.toCol + off + 1)):
						if (row == self.toRow - off or row == self.toRow + off) or (col == self.toCol - off or col == self.toCol + off):
							# Create child for each placement, then call recursively if depth limit hasn't been reached
							if canPlace and board[row][col] == " ":
								child = MinMaxNode(not(self.isMin), self.boardVal, self.alpha, self.beta, self, self.depth + 1, False, row, col)
												
								self.children = self.children + [child]

								# Keep going if possible, otherwise evaluate leaves
								if child.depth < depthLimit:
									child.generate_children()
								else:
									child.evaluate()
									child.value = child.boardVal

									# Preemptive pruning; doesn't explore a branch if it gives an opponent a winning move
									if child.value == (-1) ** int(rootIsMin + 1) * float('inf'): # -inf if root max, inf if root min
										self.value = child.value
										pruned = True
										break

								# Alpha-beta
								if self.isMin:
									self.value = min(self.value, child.value)
									self.beta = min(self.beta, self.value)
								else:
									self.value = max(self.value, child.value)
									self.alpha = max(self.alpha, self.value)

								# Cutoff if possible
								if self.beta <= self.alpha:
									pruned = True
									break

							# Create child for each movement, then call recursively if depth limit hasn't been reached
							elif canMove and board[row][col] == friendlyToken:
								for destRow in range(row-1, row+2):
									for destCol in range(col-1, col+2):
										if destRow >= 0 and destCol >= 0 and destRow < 10 and destCol < 12 and not(destRow == row and destCol == col):
											if board[destRow][destCol] == " ":
												child = MinMaxNode(not(self.isMin), self.boardVal, self.alpha, self.beta, self, self.depth + 1, True, destRow, destCol, row, col)
																
												self.children = self.children + [child]

												# Keep going if possible, otherwise evaluate leaves
												if child.depth < depthLimit:
													child.generate_children()
												else:
													child.evaluate()
													child.value = child.boardVal

													# Preemptive pruning; doesn't explore a branch if it gives an opponent a winning move
													if child.value == (-1) ** int(rootIsMin + 1) * float('inf'): # -inf if root max, inf if root min
														self.value = child.value
														pruned = True
														break

												# Alpha-beta
												if self.isMin:
													self.value = min(self.value, child.value)
													self.beta = min(self.beta, self.value)
												else:
													self.value = max(self.value, child.value)
													self.alpha = max(self.alpha, self.value)

												# Cutoff if possible
												if self.beta <= self.alpha:
													pruned = True
													break

									if pruned:
										break
								if pruned:
									break

					if pruned:
						break
				if pruned:
					break

		# Remove turn from board if not root
		if self.depth > 0:
			board[self.toRow][self.toCol] = " "
			if self.isMove:
				board[self.fromRow][self.fromCol] = opponentToken

	# Evaluate the value of a gamestate by checking the node's inherited boardVal based on the move it describes
	def evaluate(self):
		# Determine which token is opponent
		opponentToken = ""
		if self.isMin:
			opponentToken = maxToken
		else:
			opponentToken = minToken

		# Evaluate in 3x3 subgrids to find Xs and crosses
		# Process move origin first if necessary
		if self.isMove:
			# Apply move's initial token removal
			board[self.fromRow][self.fromCol] = " "

			for row in range(max(1, self.fromRow - 1), min(9, self.fromRow + 2)):
				for col in range(max(1, self.fromCol - 1), min(11, self.fromCol + 2)):
					# Make sure the subgrid isn't up or down
					if not(row != self.fromRow and col == self.fromCol):

						# Initialize counters
						maxX = 0
						minX = 0
						maxCross = 0
						minCross = 0

						# Check for Xs
						# Bottom left
						if board[row-1][col-1] == maxToken:
							maxX += 1
						elif board[row-1][col-1] == minToken:
							minX += 1

						# Top left
						if board[row+1][col-1] == maxToken:
							maxX += 1
						elif board[row+1][col-1] == minToken:
							minX += 1

						# Center
						if board[row][col] == maxToken:
							maxX += 1
						elif board[row][col] == minToken:
							minX += 1

						# Bottom right
						if board[row-1][col+1] == maxToken:
							maxX += 1
						elif board[row-1][col+1] == minToken:
							minX += 1

						# Top right
						if board[row+1][col+1] == maxToken:
							maxX += 1
						elif board[row+1][col+1] == minToken:
							minX += 1

						# Blocked Xs have no value
						if (maxX > 0 and minX == 0) or (maxX == 0 and minX > 0):
							# Get the value of subgrid based on if X has been crossed
							newVal = 0
							oldVal = 0

							# If a viable partial X exists, check if it's crossed
							if maxX > 0:
								# Check for min cross
								if board[row][col-1] == minToken:
									minCross += 1
								if board[row][col+1] == minToken:
									minCross += 1

								# Check if left or right subgrid; remove one crossing token if parent is min
								if row == self.fromRow and col != self.fromCol and not self.isMin:
									minCross += 1
									if maxX == 5:
										oldVal = float('inf') # win
									else:
										oldVal = factorial(maxX)

										# Partial crosses halve value
										if minCross == 1 and maxCross == 0:
											oldVal /= 2
									minCross -= 1

								# Crossed Xs have no value
								if minCross < 2:
									# Check if corner or mid subGrid; remove one X token if parent is max
									if ((row == self.fromRow and col == self.fromCol) or (row != self.fromRow and col != self.fromCol)) and self.isMin:
										maxX += 1
										oldVal = factorial(maxX)

										# Partial crosses halve value
										if minCross == 1 and maxCross == 0:
											oldVal /= 2
										maxX -= 1

									if maxX == 5:
										newVal = float('inf') # win
									else:
										newVal = factorial(maxX)

										# Partial crosses halve value
										if minCross == 1 and maxCross == 0:
											newVal /= 2

								# Add to value
								self.boardVal -= oldVal
								self.boardVal += newVal
							elif minX > 0:
								# Check for max cross
								if board[row][col-1] == maxToken:
									maxCross += 1
								if board[row][col+1] == maxToken:
									maxCross += 1

								# Check if left or right subgrid; remove one crossing token if parent is max
								if row == self.fromRow and col != self.fromCol and self.isMin:
									maxCross += 1
									if minX == 5:
										oldVal = float('inf') # win
									else:
										oldVal = factorial(minX)

										# Partial crosses halve value
										if maxCross == 1 and minCross == 0:
											oldVal /= 2
									maxCross -= 1

								# Crossed Xs have no value
								if maxCross < 2:
									# Check if corner or mid subGrid; remove one X token if parent is min
									if ((row == self.fromRow and col == self.fromCol) or (row != self.fromRow and col != self.fromCol)) and not self.isMin:
										minX += 1
										oldVal = factorial(minX)

										# Partial crosses halve value
										if maxCross == 1 and minCross == 0:
											oldVal /= 2
										minX -= 1

									if minX == 5:
										newVal = float('inf') # win
									else:
										newVal = factorial(minX)

										# Partial crosses halve value
										if maxCross == 1 and minCross == 0:
											newVal /= 2

								# Subtract from value
								self.boardVal += oldVal
								self.boardVal -= newVal

		# Add token at destination
		board[self.toRow][self.toCol] = opponentToken

		# Process node destination
		for row in range(max(1, self.toRow - 1), min(9, self.toRow + 2)):
			for col in range(max(1, self.toCol - 1), min(11, self.toCol + 2)):
				# Make sure the subgrid isn't up or down
				if not(row != self.toRow and col == self.toCol):

					# Initialize counters
					maxX = 0
					minX = 0
					maxCross = 0
					minCross = 0

					# Check for Xs
					# Bottom left
					if board[row-1][col-1] == maxToken:
						maxX += 1
					elif board[row-1][col-1] == minToken:
						minX += 1

					# Top left
					if board[row+1][col-1] == maxToken:
						maxX += 1
					elif board[row+1][col-1] == minToken:
						minX += 1

					# Center
					if board[row][col] == maxToken:
						maxX += 1
					elif board[row][col] == minToken:
						minX += 1

					# Bottom right
					if board[row-1][col+1] == maxToken:
						maxX += 1
					elif board[row-1][col+1] == minToken:
						minX += 1

					# Top right
					if board[row+1][col+1] == maxToken:
						maxX += 1
					elif board[row+1][col+1] == minToken:
						minX += 1

					# Blocked Xs have no value
					if (maxX > 0 and minX == 0) or (maxX == 0 and minX > 0):
						# Get the value of subgrid based on if X has been crossed
						newVal = 0
						oldVal = 0

						# If a viable partial X exists, check if it's crossed
						if maxX > 0:
							# Check for min cross
							if board[row][col-1] == minToken:
								minCross += 1
							if board[row][col+1] == minToken:
								minCross += 1

							# Check if left or right subgrid; remove one crossing token if  min
							if row == self.toRow and col != self.toCol and self.isMin:
								minCross -= 1
								if maxX == 5:
									oldVal = float('inf') # win
								else:
									oldVal = factorial(maxX)

									# Partial crosses halve value
									if minCross == 1 and maxCross == 0:
										oldVal /= 2
								minCross += 1

							# Crossed Xs have no value
							if minCross < 2:
								# Check if corner or mid subGrid; remove one X token if max
								if ((row == self.toRow and col == self.toCol) or (row != self.toRow and col != self.toCol)) and not self.isMin:
									maxX -= 1
									if maxX > 0:
										oldVal = factorial(maxX)

									# Partial crosses halve value
									if minCross == 1 and maxCross == 0:
										oldVal /= 2
									maxX += 1

								if maxX == 5:
									newVal = float('inf') # win
								else:
									newVal = factorial(maxX)

									# Partial crosses halve value
									if minCross == 1 and maxCross == 0:
										newVal /= 2

							# Add to value
							self.boardVal -= oldVal
							self.boardVal += newVal
						elif minX > 0:
							# Check for max cross
							if board[row][col-1] == maxToken:
								maxCross += 1
							if board[row][col+1] == maxToken:
								maxCross += 1

							# Check if left or right subgrid; remove one crossing token if max
							if row == self.toRow and col != self.toCol and not self.isMin:
								maxCross -= 1
								if minX == 5:
									oldVal = float('inf') # win
								else:
									oldVal = factorial(minX)

									# Partial crosses halve value
									if maxCross == 1 and minCross == 0:
										oldVal /= 2
								maxCross += 1

							# Crossed Xs have no value
							if maxCross < 2:
								# Check if corner or mid subGrid; remove one X token if min
								if ((row == self.toRow and col == self.toCol) or (row != self.toRow and col != self.toCol)) and self.isMin:
									minX -= 1
									if minX > 0:
										oldVal = factorial(minX)

									# Partial crosses halve value
									if maxCross == 1 and minCross == 0:
										oldVal /= 2
									minX += 1

								if minX == 5:
									newVal = float('inf') # win
								else:
									newVal = factorial(minX)

									# Partial crosses halve value
									if maxCross == 1 and minCross == 0:
										newVal /= 2

							# Subtract from value
							self.boardVal += oldVal
							self.boardVal -= newVal

		# Remove turn from board
		board[self.toRow][self.toCol] = " "
		if self.isMove:
			board[self.fromRow][self.fromCol] = opponentToken

class MinMaxTree:
	def __init__(self, root):
		self.root = root
		self.eval_base()

		# Run minmax on the root if the player is AI
		if not(players[currentTurn].isHuman):
			self.generate()

	def generate(self):
		self.root.generate_children()

	# Evaluate the root board to avoid redundant evaluations later
	def eval_base(self):
		# Evaluate in 3x3 subgrids to find Xs and crosses
		for row in range(1, 9):
			for col in range(1, 11):

				# Initialize counters
				maxX = 0
				minX = 0
				maxCross = 0
				minCross = 0

				# Check for Xs
				# Bottom left
				if board[row-1][col-1] == maxToken:
					maxX += 1
				elif board[row-1][col-1] == minToken:
					minX += 1

				# Top left
				if board[row+1][col-1] == maxToken:
					maxX += 1
				elif board[row+1][col-1] == minToken:
					minX += 1

				# Center
				if board[row][col] == maxToken:
					maxX += 1
				elif board[row][col] == minToken:
					minX += 1

				# Bottom right
				if board[row-1][col+1] == maxToken:
					maxX += 1
				elif board[row-1][col+1] == minToken:
					minX += 1

				# Top right
				if board[row+1][col+1] == maxToken:
					maxX += 1
				elif board[row+1][col+1] == minToken:
					minX += 1

				# Blocked Xs have no value
				if not(maxX > 0 and minX > 0):
					# Get the value of subgrid based on if X has been crossed
					subVal = 0

					# If a viable partial X exists, check if it's crossed
					if maxX > 0:
						# Check for min cross
						if board[row][col-1] == minToken:
							minCross += 1
						if board[row][col+1] == minToken:
							minCross += 1

						# Crossed Xs have no value
						if minCross < 2:
							if maxX == 5:
								subVal = float('inf') # win
							else:
								subVal = factorial(maxX)

								# Partial crosses halve value
								if minCross == 1 and maxCross == 0:
									subVal /= 2

						# Add to value
						self.root.boardVal += subVal
					elif minX > 0:
						# Check for max cross
						if board[row][col-1] == maxToken:
							maxCross += 1
						if board[row][col+1] == maxToken:
							maxCross += 1

						# Crossed Xs have no value
						if maxCross < 2:
							if minX == 5:
								subVal = float('inf') # win
							else:
								subVal = factorial(minX)

								# Partial crosses halve value
								if maxCross == 1 and minCross == 0:
									subVal /= 2

						# Subtract from value
						self.root.boardVal -= subVal

# Display board
def display_board(thisBoard):
	print(10, thisBoard[9][0],
			  thisBoard[9][1],
			  thisBoard[9][2],
			  thisBoard[9][3],
			  thisBoard[9][4],
			  thisBoard[9][5],
			  thisBoard[9][6],
			  thisBoard[9][7],
			  thisBoard[9][8],
			  thisBoard[9][9],
			  thisBoard[9][10],
			  thisBoard[9][11])
	for i in range (1, 10):
		print(10 - i, "", thisBoard[9-i][0],
						  thisBoard[9-i][1],
						  thisBoard[9-i][2],
						  thisBoard[9-i][3],
						  thisBoard[9-i][4],
						  thisBoard[9-i][5],
						  thisBoard[9-i][6],
						  thisBoard[9-i][7],
						  thisBoard[9-i][8],
						  thisBoard[9-i][9],
						  thisBoard[9-i][10],
						  thisBoard[9-i][11])
	print("   A B C D E F G H I J K L")

# Convert a string position to a pair of integer indices
def position_text_to_index(pos):
	row = 0
	col = 0

	# Assuming proper format, convert to integers
	if len(pos) == 2:
		row = int(pos[0]) - 1
		col = ord(pos[1]) - 65
	else:
		row = 9 # if length is not 2, it is 3, meaning the selected row is 10, which is equivalent to the index 9
		col = ord(pos[2]) - 65

	return (row, col)

# Convert a pair of integer indices to a string position
def position_index_to_text(row, col):
	return (str(row + 1) + chr(col + 65))

# Checks if there's a token at the position
def is_free(row, col):
	if board[row][col] == " ":
		return True
	else:
		return False

# Checks if there's a friendly token at the position
def is_friendly(row, col, friendlyToken):
	if board[row][col] == friendlyToken:
		return True
	else:
		return False

# Checks if a player has any possible moves on the board
def can_move():
	# Find tokens from the current player
	for row in range(0,10):
		for col in range(0,12):
			if board[row][col] == players[currentTurn].token:
				# Check adjacent spaces if the token can move
				if row > 0:
					if col > 0:
						if board[row-1][col-1] == " ":
							return True
					if board[row-1][col] == " ":
						return True
					if col < 11:
						if board[row-1][col+1] == " ":
							return True
				if col > 0:
					if board[row][col-1] == " ":
						return True
				if col < 11:
					if board[row][col+1] == " ":
						return True
				if row < 9:
					if col > 0:
						if board[row+1][col-1] == " ":
							return True
					if board[row+1][col] == " ":
						return True
					if col < 11:
						if board[row+1][col+1] == " ":
							return True

	return False

# Checks if pos1 is adjacent to pos2
def are_adjacent(pos1, pos2):
	indices1 = position_text_to_index(pos1)
	row1 = indices1[0]
	col1 = indices1[1]
	indices2 = position_text_to_index(pos2)
	row2 = indices2[0]
	col2 = indices2[1]

	# Tiles aren't adjacent if they have the same coordinates
	if row1 == row2 and col1 == col2:
		return False

	# Check for adjacence
	if row1 - 1 == row2 or row1 == row2 or row1 + 1 == row2:
		if col1 - 1 == col2 or col1 == col2 or col1 + 1 == col2:
			return True

	return False

# Processes a user's turn input
# OUTPUT: command accepted?, error message, move?, move toRow, move toCol, move fromRow, move fromCol
def process_input(command):
	# Validate input
	if len(command) == 2:
		# Check for [digit][letter]
		if command[0].isdigit() and command[1].isalpha():
			# True for 1A to 9L
			if int(command[0]) > 0 and ord(command[1]) - 65 >= 0 and ord(command[1]) - 65 < 12:
				toCoords = position_text_to_index(command)
				if is_free(toCoords[0], toCoords[1]):
					if players[currentTurn].tokens > 0:
						return True, "", False, toCoords[0], toCoords[1], 0, 0
					else:
						return False, "ERROR - No tokens left", False, 0, 0, 0, 0
				else:
					return False, "ERROR - Position occupied", False, 0, 0, 0, 0
	elif len(command) == 3:
		# Check for 10[letter]
		if command[0:2] == "10" and command[2].isalpha():
			# True for 10A to 10L
			if ord(command[2]) - 65 >= 0 and ord(command[2]) - 65 < 12:
				toCoords = position_text_to_index(command)
				if is_free(toCoords[0], toCoords[1]):
					if players[currentTurn].tokens > 0:
						return True, "", False, toCoords[0], toCoords[1], 0, 0
					else:
						return False, "ERROR - No tokens left", False, 0, 0, 0, 0
				else:
					return False, "ERROR - Position occupied", False, 0, 0, 0, 0
	elif len(command) == 5:
		# Check for [digit][letter] [digit][letter]
		if command[0].isdigit() and command[1].isalpha() and command[2].isspace() and command[3].isdigit() and command[4].isalpha():
			# True for 1A to 9L for both coords
			if int(command[0]) > 0 and ord(command[1]) - 65 >= 0 and ord(command[1]) - 65 < 12 and int(command[3]) > 0 and ord(command[4]) - 65 >= 0 and ord(command[4]) - 65 < 12:
				fromCoords = position_text_to_index(command[0:2])
				toCoords = position_text_to_index(command[3:5])
				if is_friendly(fromCoords[0], fromCoords[1], players[currentTurn].token):
					if is_free(toCoords[0], toCoords[1]):
						if are_adjacent(command[0:2], command[3:5]):
							if moves > 0:
								return True, "", True, toCoords[0], toCoords[1], fromCoords[0], fromCoords[1]
							else:
								return False, "ERROR - No moves left", False, 0, 0, 0, 0
						else:
							return False, "ERROR - Destination is not adjacent", False, 0, 0, 0, 0
					else:
						return False, "ERROR - Destination is not free", False, 0, 0, 0, 0
				else:
					return False, "ERROR - Not a friendly token", False, 0, 0, 0, 0
	elif len(command) == 6 and command[3].isspace():
		# Check for 10[letter] [digit][letter]
		if command[0:2] == "10" and command[2].isalpha() and command[4].isdigit() and command[5].isalpha():
			# True for 10A to 10L for both coords
			if ord(command[2]) - 65 >= 0 and ord(command[2]) - 65 < 12 and ord(command[5]) - 65 >= 0 and ord(command[5]) - 65 < 12:
				fromCoords = position_text_to_index(command[0:3])
				toCoords = position_text_to_index(command[4:6])
				if is_friendly(fromCoords[0], fromCoords[1], players[currentTurn].token):
					if is_free(toCoords[0], toCoords[1]):
						if are_adjacent(command[0:3], command[4:6]):
							if moves > 0:
								return True, "", True, toCoords[0], toCoords[1], fromCoords[0], fromCoords[1]
							else:
								return False, "ERROR - No moves left", False, 0, 0, 0, 0
						else:
							return False, "ERROR - Destination is not adjacent", False, 0, 0, 0, 0
					else:
						return False, "ERROR - Destination is not free", False, 0, 0, 0, 0
				else:
					return False, "ERROR - Not a friendly token", False, 0, 0, 0, 0
	elif len(command) == 6 and command[2].isspace():
		# Check for [digit][letter] 10[letter]
		if command[0].isdigit() and command[1].isalpha() and command[3:5] == "10" and command[5].isalpha():
			# True for 10A to 10L for both coords
			if ord(command[1]) - 65 >= 0 and ord(command[1]) - 65 < 12 and ord(command[5]) - 65 >= 0 and ord(command[5]) - 65 < 12:
				fromCoords = position_text_to_index(command[0:2])
				toCoords = position_text_to_index(command[3:6])
				if is_friendly(fromCoords[0], fromCoords[1], players[currentTurn].token):
					if is_free(toCoords[0], toCoords[1]):
						if are_adjacent(command[0:2], command[3:6]):
							if moves > 0:
								return True, "", True, toCoords[0], toCoords[1], fromCoords[0], fromCoords[1]
							else:
								return False, "ERROR - No moves left", False, 0, 0, 0, 0
						else:
							return False, "ERROR - Destination is not adjacent", False, 0, 0, 0, 0
					else:
						return False, "ERROR - Destination is not free", False, 0, 0, 0, 0
				else:
					return False, "ERROR - Not a friendly token", False, 0, 0, 0, 0
	elif len(command) == 7:
		# Check for 10[letter] 10[letter]
		if command[0:2] == "10" and command[2].isalpha() and command[3] == " " and command[4:6] == "10" and command[6].isalpha():
			# True for 10A to 10L for both coords
			if ord(command[2]) - 65 >= 0 and ord(command[2]) - 65 < 12 and ord(command[6]) - 65 >= 0 and ord(command[6]) - 65 < 12:
				fromCoords = position_text_to_index(command[0:3])
				toCoords = position_text_to_index(command[4:7])
				if is_friendly(fromCoords[0], fromCoords[1], players[currentTurn].token):
					if is_free(toCoords[0], toCoords[1]):
						if are_adjacent(command[0:3], command[4:7]):
							if moves > 0:
								return True, "", True, toCoords[0], toCoords[1], fromCoords[0], fromCoords[1]
							else:
								return False, "ERROR - No moves left", False, 0, 0, 0, 0
						else:
							return False, "ERROR - Destination is not adjacent", False, 0, 0, 0, 0
					else:
						return False, "ERROR - Destination is not free", False, 0, 0, 0, 0
				else:
					return False, "ERROR - Not a friendly token", False, 0, 0, 0, 0

	# Invalid input
	return False, "ERROR - Invalid input", False, 0, 0, 0, 0

# Lets a player take their turn
def turn(lastMoveNode):
	global currentTurn
	global moves

	# Create a node to remember the turn
	moveNode = None

	turnOver = False

	msg = "\nPlayer {}'s turn!"
	print(msg.format(currentTurn + 1))

	while not(turnOver):
		# Display board
		display_board(board)

		# Check if this player has tokens to place
		if players[currentTurn].tokens <= 0:
			if moves <= 0:
				msg = "\nPlayer {} has no more tokens and there are no more moves left! Ending turn.\n"
				print(msg.format(currentTurn + 1))
				break;
			elif not(can_move()):
				msg = "\nPlayer {} has no more tokens and cannot move! Ending turn.\n"
				print(msg.format(currentTurn + 1))
				break;

		# Give players a choice if human, otherwise evaluate the board
		if players[currentTurn].isHuman:
			# Get min/max for next player
			if currentTurn == 0:
				moveNode = MinMaxNode(False)
			else:
				moveNode = MinMaxNode(True)

			# Get action
			msg = "\nPlayer {}'s turn"
			print(msg.format(currentTurn + 1))
			msg = "Tokens left = {}"
			print(msg.format(players[currentTurn].tokens))
			msg = "Moves left = {}"
			print(msg.format(moves))
			action = input("Input command: ")

			# Ensure the input was valid
			turnEnded = False
			errMsg = ""
			while not(turnEnded):
				turnEnded, errMsg, moveNode.isMove, moveNode.toRow, moveNode.toCol, moveNode.fromRow, moveNode.fromCol = process_input(action)
				if errMsg != "":
					print(errMsg)
					action = input("Input new command: ")

			# Modify board with result and print
			board[moveNode.toRow, moveNode.toCol] = players[currentTurn].token
			if moveNode.isMove:
				moves -= 1
				board[moveNode.fromRow, moveNode.fromCol] = " "
				print(position_index_to_text(moveNode.fromRow, moveNode.fromCol) + " " + position_index_to_text(moveNode.toRow, moveNode.toCol) + "\n")
			else:
				players[currentTurn].tokens -= 1
				print(position_index_to_text(moveNode.toRow, moveNode.toCol) + "\n")

			turnOver = True

		else:
			startTime = time.time()
			# Run minmax on current gamestate
			tree = MinMaxTree(lastMoveNode)

			for child in tree.root.children:
				# Remember move and reset it for root
				moveNode = child
				moveNode.boardVal = 0
				moveNode.alpha = -float('inf')
				moveNode.beta = float('inf')
				moveNode.parent = None
				moveNode.depth = 0

				# Pick the first child that has the same value as root
				if tree.root.value == child.value:
					# Modify board
					board[child.toRow][child.toCol] = players[currentTurn].token
					if child.isMove:
						moves -= 1
						board[child.fromRow][child.fromCol] = " "
						print(position_index_to_text(child.fromRow, child.fromCol) + " " + position_index_to_text(child.toRow, child.toCol))
					else:
						players[currentTurn].tokens -= 1
						print(position_index_to_text(child.toRow, child.toCol))

					break

			# Return time in seconds with 2 decimals
			endTime = time.time() - startTime
			print("Took ", ((endTime * 100)//1)/100, " seconds.")

			# End turn
			turnOver = True

	return moveNode

# Checks if either player has won
# NOTE: Only checks for one player, since a play can make both players
#		have a winning X but the player whose turn it was has priority
def check_win(playerIndex):
	# Set tokens
	currentToken = players[playerIndex].token
	otherToken = ""
	if currentToken == "■":
		otherToken = "□"
	else:
		otherToken = "■"

	# NOTE: Look for X centers; no need to check edges
	for row in range(1, 9):
		for col in range(1, 11):
			if board[row][col] == currentToken:
				# Check for friendly corners and opposing tokens to the side
				if (board[row-1][col-1] == currentToken and
					board[row-1][col+1] == currentToken and
					board[row+1][col-1] == currentToken and
					board[row+1][col+1] == currentToken and
					not(board[row][col-1] == otherToken and board[row][col+1] == otherToken)):
				   	# If a winning X was found, declare this player the winner
				   	return True

	# If no winning X was found for this player, return false
	return False

# Check if there has been a draw
def check_draw():
	# Check if no win has been achieved and no more actions are possible
	if (moves <= 0 and 
		players[0].tokens <= 0 and 
		players[1].tokens <= 0 and
		not(check_win(0)) and
		not(check_win(1))):

		return True
	else:
		return False

# Checks for the game state. 0 = ongoing, 1 = last turn won, 2 = last turn lost, 3 = draw
def gamestate(lastTurnIndex):
	nextTurnIndex = 0
	if lastTurnIndex == 0:
		nextTurnIndex = 1

	# Check for a win
	if check_win(lastTurnIndex):
		return 1
	# Check for a loss
	elif check_win(nextTurnIndex):
		return 2
	# Check for a draw
	elif check_draw():
		return 3
	# Ongoing game
	else:
		return 0

# Reset the game's state
def reset_game():
	global board
	global currentTurn
	global moves

	# Reset variables
	currentTurn = 0
	players[0].reset(True)
	players[1].reset(True)
	moves = 30

	# Reset board
	board = np.array(
		[(" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," "),
		 (" "," "," "," "," "," "," "," "," "," "," "," ")])

# Game mode selection
def main_menu():
	print("Select game mode:")
	print("1 - Human vs Human")
	print("2 - Human vs AI")

	# Get game mode
	mode = input("Game mode: ")

	# Check input
	while mode != "1" and mode != "2":
		mode = input("ERROR - Invalid input. Select game mode: ")
	print()

	# Set NPC if necessary
	if mode == "2":
		print("Select human player:")
		print("1 - Player 1")
		print("2 - Player 2")

		# Select human player
		human = input("Human player: ")

		# Check input
		while human != "1" and human != "2":
			human = input("ERROR - Invalid input. Select human player: ")

		if human == "1":
			# Player 1 human, so 2 is AI
			players[1].isHuman = False
		else:
			# Player 2 human, so 1 is AI
			players[0].isHuman = False

# Greet the player and open the main menu
print("Welcome to X-Rudder!\n")
main_menu()

# Set up the game
endgame = False

# Create a root min node
lastMoveNode = MinMaxNode(True)

while not(endgame):

	# Play a turn and display the board again
	lastMoveNode = turn(lastMoveNode)

	# Check if the game is over
	state = gamestate(currentTurn)

	# Announce a draw
	if state == 3:
		print("\nThe game is a draw!!!")
		endgame = True

	# Announce a victory
	vicMsg = "\nPlayer {} has won!!!"

	if state == 1:
		print(vicMsg.format(currentTurn + 1))
		endgame = True

	# Change player turn
	if currentTurn == 0:
		currentTurn = 1
	else:
		currentTurn = 0
	
	# Announce a loss (Do this after changing player turns to avoid redundancy)
	if state == 2:
		print(vicMsg.format(currentTurn + 1))
		endgame = True

	# Check if user would like to play again.
	if endgame:
		display_board(board)
		again = input("\nWould you like to play another match? (y/n): ")

		while again != "y" and again != "n":
			print("ERROR - Invalid input. Would you like to play another match? (y/n): ")
			again = input()

		if again == "y":
			endgame = False
			reset_game()
			main_menu()
			display_board(board)

print("\nThank you for playing X-Rudder!")