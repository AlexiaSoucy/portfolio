using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyBulletBehaviour : MonoBehaviour
{
    // Movement variables
    public Rigidbody2D rb;
    public float moveSpeed;

    // Start is called before the first frame update
    void Start()
    {
        // Move forward
        rb.velocity = transform.up * moveSpeed;
    }

    private void OnTriggerEnter2D(Collider2D other) {
        if (other.gameObject.tag == "Player")
        {
            // Take damage
            other.gameObject.SendMessage("LoseLife");

            // Clean
            Destroy(gameObject);
        }
        else if (other.gameObject.tag == "Wall")
            Destroy(gameObject);
    }
}
