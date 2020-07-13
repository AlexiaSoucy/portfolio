using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerBehaviour : MonoBehaviour
{
    // Gamestate variables
    private int life = 3;
    private bool whiteMode = false;
    private float meter = 0f;
    public float meterFillRate;
    public float meterDrainRate;
    public ParticleSystem meterFullPS;
    public GameObject env;
    public GameObject shieldR;
    public GameObject shieldG;
    public GameObject shieldB;

    // Movement variables
    public Rigidbody2D rb;
    public float moveSpeed;

    // Shooting variables
    public GameObject bulletPrefab;
    public GameObject whiteBulletPrefab;
    public float shootDelay;
    private float shootCountdown = 0f;
    public Transform bulletSpawn;
    private float hue = 0; // Angle in HSV colours, where 0/360 = red.

    // Interface variables
    public GameObject selector;
    public Image selectorImage;
    public Sprite whiteSelector;
    public Sprite blackSelector;
    public GameObject life1;
    public GameObject life2;
    public GameObject life3;
    public GameObject life4;
    public GameObject life5;
    private GameObject[] lives;
    public RectTransform meterFill;
    public float meterMinScale;
    public float meterMaxScale;

    private void Start() {
        lives = new GameObject[]{life1, life2, life3, life4, life5};
    }

    // Update is called once per frame
    void Update()
    {
        // Only allow colour selection outside of white mode
        if (!whiteMode)
        {
            float scrollVal = Input.GetAxis("Scroll");
            if (scrollVal != 0f)
            {
                // Select new colour
                ChangeHue(scrollVal);

                // Show change on interface
                float degrees = scrollVal*1200;
                selector.SendMessage("Rotate", degrees);
            }
        }

        // Spacebar drops shield and uses 25% meter
        if (Input.GetButtonDown("Space") && !whiteMode && meter >= 0.25f)
        {
            // Use resource
            meter -= 0.25f;

            // Turn off meter particles (not displaying properly in webGL; deactivating for web player)
            //meterFullPS.Stop();

            // Spawn shield based on hue
            switch (hue)
            {
                case 120:
                    Instantiate(shieldG, transform.position, transform. rotation);
                    break;
                case 240:
                    Instantiate(shieldB, transform.position, transform.rotation);
                    break;
                default:
                    Instantiate(shieldR, transform.position, transform.rotation);
                    break;
            }
        }

        // Activate white mode
        if (Input.GetButtonDown("White") && meter == 1f)
        {
            whiteMode = true;

            // Turn off the meter particles (not displaying properly in webGL; deactivating for web player)
            //meterFullPS.Stop();

            // Show selector particles (not displaying properly in webGL; deactivating for web player)
            //selector.SendMessage("PlayWhite");

            // Switch selector image
            selectorImage.sprite = whiteSelector;
        }

        // Count down until the next shot can be fired
        if (shootCountdown > 0f)
            shootCountdown -= Time.deltaTime;

        // Shoot on command if able
        if (Input.GetButton("Shoot") && shootCountdown <= 0f)
            Shoot();

        if (whiteMode && meter > 0f)
        {
            // Remove from meter if over 0%
            meter -= meterDrainRate * Time.deltaTime;
            meter = Mathf.Max(meter, 0f);

            if (meter == 0f)
            {
                // End white mode
                whiteMode = false;

                // Hide selector particles (not displaying properly in webGL; deactivating for web player)
                //selector.SendMessage("StopWhite");

                // Return the interface to normal
                selectorImage.sprite = blackSelector;
            }
        }
        else if (!whiteMode && meter < 1f)
        {
            // Add to meter if less than 100% (1)
            meter += meterFillRate * Time.deltaTime;
            meter = Mathf.Min(meter, 1f);

            if (meter == 1f)
            {
                // (not displaying properly in webGL; deactivating for web player)
                //meterFullPS.Play();
            }
        }
            
        // Interpolate the position of the meter sprite
        float meterScale = (1 - meter) * meterMinScale + meter * meterMaxScale;
        meterFill.sizeDelta = new Vector2(meterScale, meterFill.sizeDelta.y);

        // Every frame, center the camera on the player
        Camera.main.transform.position = new Vector3(transform.position.x, transform.position.y, Camera.main.transform.position.z);
    }

    private void FixedUpdate() 
    {
        // Handle movement controls (in FixedUpdate as RigidBodies are involved)
        Move();
        Rotate();
    }

    private void OnDestroy() {
        env.SendMessage("StopCounting");
    }

    private void Move()
    {
        // Take WASD input as movement and combine with movement speed
        rb.velocity = new Vector2(Input.GetAxis("Horizontal"), Input.GetAxis("Vertical")) * moveSpeed;
    }

    private void Rotate()
    {
        // Find the direction to face in based on mouse position
        Vector3 lookPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        Vector2 direction = (Vector2)lookPos - rb.position;

        // Change player orientation
        rb.transform.up = direction;
    }

    private void Shoot()
    {
        // Begin shot countdown
        shootCountdown = shootDelay;

        // Check if white mode on
        if (whiteMode)
        {
            // Create a new bullet in the appropriate location
            GameObject bullet = Instantiate(whiteBulletPrefab, bulletSpawn.position, bulletSpawn.rotation);
        }
        else
        {
            // Create a new bullet in the appropriate location
            GameObject bullet = Instantiate(bulletPrefab, bulletSpawn.position, bulletSpawn.rotation);

            // Set bullet color
            SpriteRenderer sr = bullet.GetComponent<SpriteRenderer>();
            sr.color = Color.HSVToRGB(hue/360, 1, 1);
        }
    }

    private void ChangeHue(float amount)
    {
        /*
        - Primary colours are 120 degrees apart
        - Mouse wheel input is processed in steps of 0.1
        - Result: multiplier is 1200
        */
        float degChange = amount * 1200;

        // Change hue value and make sure it stays in the 0-360 range
        hue += degChange;
        if (hue < 0)
        {
            hue += 360;
        }
        else if (hue > 360)
        {
            hue -= 360;
        }
    }

    public void LoseLife()
    {
        // Remove 1 life icon
        lives[life - 1].SetActive(false);

        // Take 1 damage
        life--;

        if (life == 0)
            Destroy(gameObject);
    }

    public void GainLife()
    {
        if (life < 5)
        {
            // Gain 1 life
            life++;

            // Add 1 life icon
            lives[life-1].SetActive(true);
        }
    }

    public void GetPoints(int p)
    {
        meter += p/100f;
        meter = Mathf.Min(meter, 1f);
    }
}
