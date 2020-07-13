using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EmitterBehaviour : MonoBehaviour
{
    // Shooting variables
    public GameObject bulletPrefab;
    public float shootDelay;
    private float shootCountdown = 0f;
    public Transform bulletSpawn;
    public int shotCount;

    // Movement variables
    public float rotateSpeed;
    public Rigidbody2D rb;
    public GameObject pointerPrefab;
    private GameObject pointer;

    private void Start() {
        shootCountdown = shootDelay;

        // Find player
        GameObject player = GameObject.FindGameObjectWithTag("Player");

        // Create pointer and place it on the player
        pointer = Instantiate(pointerPrefab, player.transform.position, player.transform.rotation);
        pointer.transform.parent = player.transform;
    }

    private void Update() {
        // Count down until the next shot can be fired
        if (shootCountdown > 0f)
            shootCountdown -= Time.deltaTime;

        // Shoot if able
        if (shootCountdown <= 0f)
        {
            // Shoot whenever possible
            Shoot();
        }

        pointer.SendMessage("SetTarget", transform.position);
    }

    private void FixedUpdate() {
        // Rotate in place
        rb.angularVelocity = rotateSpeed;
    }

    private void Shoot()
    {
        // Begin shot countdown
        shootCountdown = shootDelay;

        for (float a = 0; a < 360f; a+=360f/shotCount)
        {
            // Create a new bullet in the appropriate location
            Transform spawn = bulletSpawn;
            spawn.RotateAround(gameObject.transform.position, Vector3.forward, a);
            GameObject bullet = Instantiate(bulletPrefab, spawn.position, spawn.rotation);

            // Set bullet color
            //SpriteRenderer sr = bullet.GetComponent<SpriteRenderer>();
            //sr.color = Color.HSVToRGB(hue/360, 1, 1);
        }
    }

    private void OnDestroy() {
        if (pointer != null)
            pointer.SendMessage("Die");
    }

    private void OnTriggerEnter2D(Collider2D other) {
        if (other.gameObject.tag == "Player")
        {
            other.gameObject.SendMessage("LoseLife");
            Destroy(gameObject);
        }
    }
}
