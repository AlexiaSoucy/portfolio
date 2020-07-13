using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WhiteBulletBehaviour : MonoBehaviour
{
    // Movement variables
    public Rigidbody2D rb;
    public float moveSpeed;

    // Shooting variables
    private GameObject player;

    // Start is called before the first frame update
    void Start()
    {
        // Assign player
        player = GameObject.FindGameObjectWithTag("Player");

        // Move forward
        rb.velocity = transform.up * moveSpeed;
    }

    private void OnTriggerEnter2D(Collider2D other) {
        if (other.gameObject.tag == "Bullet")
        {
            Destroy(other.gameObject);
            Destroy(gameObject);
        }
        if (other.gameObject.tag == "Blaster" || other.gameObject.tag == "Charger" || other.gameObject.tag == "Emitter" || 
            other.gameObject.tag == "BlasterB" || other.gameObject.tag == "ChargerB" || other.gameObject.tag == "EmitterB")
        {
            // Gain points for kill
            if (other.gameObject.tag == "Charger")
            {
                player.SendMessage("GetPoints", 5);
            }
            else if (other.gameObject.tag == "Blaster")
            {
                player.SendMessage("GetPoints", 10);
            }
            else if (other.gameObject.tag == "Emitter")
            {
                player.SendMessage("GetPoints", 15);
            }
            else if (other.gameObject.tag == "ChargerB")
            {
                player.SendMessage("GetPoints", 10);
            }
            else if (other.gameObject.tag == "BlasterB")
            {
                player.SendMessage("GetPoints", 15);
            }
            else if (other.gameObject.tag == "EmitterB")
            {
                player.SendMessage("GetPoints", 20);
            }

            // Clean
            Destroy(other.gameObject);
            Destroy(gameObject);
        }
        else if (other.gameObject.tag == "Wall")
            Destroy(gameObject);
    }
}
