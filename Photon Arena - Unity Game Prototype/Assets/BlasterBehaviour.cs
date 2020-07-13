using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlasterBehaviour : MonoBehaviour
{
    // Shooting variables
    public GameObject bulletPrefab;
    public float shootDelay;
    private float shootCountdown;
    public Transform bulletSpawn;

    // Movement variables
    public float moveSpeed;
    public float rotateSpeed;
    public Rigidbody2D rb;
    private GameObject player;
    public float distanceMin;
    public float distanceMax;
    public GameObject pointerPrefab;
    private GameObject pointer;

    // Start is called before the first frame update
    void Start()
    {
        shootCountdown = shootDelay;

        // Find player
        player = GameObject.FindGameObjectWithTag("Player");

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
            // Shoot if possible
            if (player != null)
                Shoot();
        }
    }

    private void FixedUpdate() {
        if (player != null)
        {
            // Get normalized target direction
            Vector2 direction = (Vector2)player.transform.position - rb.position;
            direction.Normalize();

            // Use cross-product to determine necessary rotation to face player
            float rotation = Vector3.Cross(direction, transform.up).z;

            // Change orientation (uses angularVelocity instead of just changing the orientation directly in order to have it gradually rotate)
            rb.angularVelocity = -rotation * rotateSpeed;

            // Move forward if further than desired range, backwards if too close
            float distance = Vector2.Distance(rb.position, (Vector2)player.transform.position);
            if (distance <= distanceMin)
                rb.AddForce(-transform.up * moveSpeed);
            else if (distance >= distanceMax)
                rb.AddForce(transform.up * moveSpeed);

            pointer.SendMessage("SetTarget", transform.position);
        }
    }

    public void Shoot()
    {
        // Begin shot countdown
        shootCountdown = shootDelay;

        // Create a new bullet in the appropriate location
        GameObject bullet = Instantiate(bulletPrefab, bulletSpawn.position, bulletSpawn.rotation);

        // Set bullet color
        //SpriteRenderer sr = bullet.GetComponent<SpriteRenderer>();
        //sr.color = Color.HSVToRGB(hue/360, 1, 1);
    }

    private void OnDestroy() {
        if (pointer != null)
            pointer.SendMessage("Die");
    }

    private void OnCollisionEnter2D(Collision2D other) {
        if (other.gameObject.tag == "Player")
        {
            other.gameObject.SendMessage("LoseLife");
            Destroy(gameObject);
        }
    }
}
