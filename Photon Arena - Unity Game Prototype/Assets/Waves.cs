using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Waves : MonoBehaviour
{
    //  State variables
    private int wave = -1;
    private float timer = 0f;
    public Text waveText;
    public Text timerText;
    private bool counting = true;
    public GameObject player;
    private bool paused = false;
    public GameObject pauseMenu;

    // Spawning variables
    public Transform spawnN;
    public Transform spawnNE;
    public Transform spawnNW;
    public Transform spawnS;
    public Transform spawnSE;
    public Transform spawnSW;
    public Transform spawnBNE;
    public Transform spawnBNW;
    public Transform spawnE;
    public Transform spawnW;
    public Transform spawnBSE;
    public Transform spawnBSW;
    public GameObject blaster;
    public GameObject charger;
    public GameObject emitter;
    public GameObject blasterB;
    public GameObject chargerB;
    public GameObject emitterB;

    private void Start() {
        // Start first wave
        nextWave();
    }

    private void Update() {
        if (counting)
        {
            // Update timer
            timer -= Time.deltaTime;
            timer = Mathf.Max(timer, 0f);

            // Update timer text in correct format
            int minutes = (int)(timer/60);
            int seconds = (int)((timer%60));
            int hundredths = (int)(timer%1 * 100);
            if (minutes >= 10 && seconds >= 10 && hundredths >= 10)
                timerText.text = minutes.ToString() + ":" + seconds.ToString() + ":" + hundredths.ToString();
            else if (minutes >= 10 && seconds >= 10 && hundredths < 10)
                timerText.text = minutes.ToString() + ":" + seconds.ToString() + ":0" + hundredths.ToString();
            else if (minutes >= 10 && seconds < 10 && hundredths >= 10)
                timerText.text = minutes.ToString() + ":0" + seconds.ToString() + ":" + hundredths.ToString();
            else if (minutes >= 10 && seconds < 10 && hundredths < 10)
                timerText.text = minutes.ToString() + ":0" + seconds.ToString() + ":0" + hundredths.ToString();
            else if (minutes < 10 && seconds >= 10 && hundredths >= 10)
                timerText.text = "0" + minutes.ToString() + ":" + seconds.ToString() + ":" + hundredths.ToString();
            else if (minutes < 10 && seconds >= 10 && hundredths < 10)
                timerText.text = "0" + minutes.ToString() + ":" + seconds.ToString() + ":0" + hundredths.ToString();
            else if (minutes < 10 && seconds < 10 && hundredths >= 10)
                timerText.text = "0" + minutes.ToString() + ":0" + seconds.ToString() + ":" + hundredths.ToString();
            else if (minutes < 10 && seconds < 10 && hundredths < 10)
                timerText.text = "0" + minutes.ToString() + ":0" + seconds.ToString() + ":0" + hundredths.ToString();

            if (timer == 0f)
                nextWave();
        }

        // Pause control
        if (Input.GetButtonDown("Escape"))
        {
            if (paused)
                Unpause();
            else
                Pause();
        }
    }

    private void nextWave()
    {
        // Up wave count
        wave++;

        // Display wave num
        waveText.text = wave.ToString();

        // Spawn wave
        switch (wave)
        {
            case 0:
                // Prep time
                timer = 5f;
                break;
            case 1:
                // Show player a basic enemy
                Spawn(blaster, spawnN, 180);
                timer = 5f;
                break;
            case 2:
                // Introduce hues
                Spawn(blaster, spawnN, 180);
                Spawn(blaster, spawnNE, 60);
                Spawn(blaster, spawnNW, 300);
                timer = 5f;
                break;
            case 3:
                // Introduce chargers
                Spawn(charger, spawnBNE, 180);
                Spawn(charger, spawnBNW, 180);
                Spawn(charger, spawnE, 60);
                Spawn(charger, spawnW, 60);
                Spawn(charger, spawnBSE, 300);
                Spawn(charger, spawnBSW, 300);
                timer = 5f;
                break;
            case 4:
                // Introduce emmitters
                Spawn(emitter, spawnE, 180);
                Spawn(emitter, spawnBNW, 300);
                Spawn(emitter, spawnBSW, 60);
                timer = 10f;
                break;
            case 5:
                // Intro black enemies
                Spawn(emitterB, spawnN);
                Spawn(emitterB, spawnSW);
                Spawn(emitterB, spawnSE);
                timer = 15f;
                break;
            case 6:
                player.SendMessage("GainLife");
                // Introduce RGB enemies
                Spawn(blaster, spawnW, 0);
                Spawn(blaster, spawnBNE, 240);
                Spawn(blaster, spawnBSE, 120);
                timer = 30f;
                break;
            case 7:
                Spawn(charger, spawnBNE, 180);
                Spawn(charger, spawnBNW, 180);
                Spawn(charger, spawnE, 60);
                Spawn(charger, spawnW, 60);
                Spawn(charger, spawnBSE, 300);
                Spawn(charger, spawnBSW, 300);
                Spawn(blaster, spawnBNE, 180);
                Spawn(blaster, spawnBNW, 180);
                Spawn(blaster, spawnE, 60);
                Spawn(blaster, spawnW, 60);
                Spawn(blaster, spawnBSE, 300);
                Spawn(blaster, spawnBSW, 300);
                Spawn(charger, spawnBNE, 0);
                Spawn(charger, spawnBNW, 0);
                Spawn(charger, spawnE, 120);
                Spawn(charger, spawnW, 120);
                Spawn(charger, spawnBSE, 240);
                Spawn(charger, spawnBSW, 240);
                timer = 5f;
                break;
            case 8:
                Spawn(blaster, spawnE, 180);
                Spawn(blaster, spawnW, 180);
                Spawn(blaster, spawnBNE, 300);
                Spawn(blaster, spawnBNW, 300);
                Spawn(blaster, spawnBSE, 60);
                Spawn(blaster, spawnBSW, 60);
                timer = 10f;
                break;
            case 9:
                Spawn(emitter, spawnN, 0);
                Spawn(emitter, spawnS, 180);
                Spawn(emitter, spawnNE, 60);
                Spawn(emitter, spawnNW, 300);
                Spawn(emitter, spawnSE, 120);
                Spawn(emitter, spawnSW, 240);
                timer = 60f;
                break;
            case 10:
                Spawn(chargerB, spawnE);
                Spawn(chargerB, spawnE);
                Spawn(chargerB, spawnE);
                Spawn(chargerB, spawnE);
                Spawn(chargerB, spawnE);
                Spawn(chargerB, spawnW);
                Spawn(chargerB, spawnW);
                Spawn(chargerB, spawnW);
                Spawn(chargerB, spawnW);
                Spawn(chargerB, spawnW);
                timer = 5f;
                break;
            case 11:
                Spawn(blasterB, spawnN);
                Spawn(blasterB, spawnSE);
                Spawn(blasterB, spawnSW);
                break;
            case 12:
                // Loop
                player.SendMessage("GainLife");
                timer = 5f;
                wave = 0;
                break;
        }
    }

    // Spawning function for coloured enemies
    private void Spawn(GameObject prefab, Transform spawnPoint, float hue)
    {
        // change hue before spawning
        HueSwitcher hw = prefab.GetComponent<HueSwitcher>();
        hw.hue = hue;

        // spawn
        GameObject enemy = Instantiate(prefab, spawnPoint.position, spawnPoint.rotation);
    }

    // Spawning function for black enemies; doesn't manage hue.
    private void Spawn(GameObject prefab, Transform spawnPoint)
    {
        // spawn
        GameObject enemy = Instantiate(prefab, spawnPoint.position, spawnPoint.rotation);
    }

    public void StopCounting()
    {
        counting = false;
    }

    public void Pause()
    {
        paused = true;

        pauseMenu.SetActive(true);
    }

    public void Unpause()
    {
        paused = false;

        pauseMenu.SetActive(false);
    }
}
