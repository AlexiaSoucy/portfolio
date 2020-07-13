using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChargerBehaviour : MonoBehaviour
{
    // Movement variables
    public float moveSpeed;
    public float rotateSpeed;
    public Rigidbody2D rb;
    private GameObject player;
    public GameObject pointerPrefab;
    private GameObject pointer;

    // Start is called before the first frame update
    void Start()
    {
        // Find player
        player = GameObject.FindGameObjectWithTag("Player");

        // Create pointer and place it on the player
        pointer = Instantiate(pointerPrefab, player.transform.position, player.transform.rotation);
        pointer.transform.parent = player.transform;
    }

    private void Update() {
        pointer.SendMessage("SetTarget", transform.position);
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

            // Move forward unless the target is behind
            rb.velocity = transform.up * moveSpeed;
        }
    }

    private void OnTriggerEnter2D(Collider2D other) {
        if (other.gameObject.tag == "Player")
        {
            other.gameObject.SendMessage("LoseLife");
            Destroy(gameObject);
        }
        else if (other.gameObject.tag == "Wall")
        {
            Destroy(gameObject);
        }
    }

    private void OnDestroy() {
        if (pointer != null)
            pointer.SendMessage("Die");
    }
}
