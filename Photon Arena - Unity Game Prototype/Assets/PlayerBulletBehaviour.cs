using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerBulletBehaviour : MonoBehaviour
{
    // Movement variables
    public Rigidbody2D rb;
    public float moveSpeed;

    // Shooting variables
    public SpriteRenderer sr;
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
        if (other.gameObject.tag == "Blaster" || other.gameObject.tag == "Charger" || other.gameObject.tag == "Emitter" || other.gameObject.tag == "Shield")
        {
            // Get both sprites' colour data
            float h, s, v;
            float oh, os, ov;
            SpriteRenderer otherSR = other.gameObject.GetComponent<SpriteRenderer>();

            Color.RGBToHSV(sr.color, out h, out s, out v);
            Color.RGBToHSV(otherSR.color, out oh, out os, out ov);

            // Convert fractions to degrees
            h *= 360;
            oh *= 360;

            if (other.gameObject.tag == "Blaster" || other.gameObject.tag == "Charger" || other.gameObject.tag == "Emitter")
            {
                // Destroy the enemy and the bullet if they are complements
                if (Mathf.Abs(oh - h) == 180)
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

                    // Clean
                    Destroy(other.gameObject);
                    Destroy(gameObject);
                }
            }
            else if (other.gameObject.tag == "Shield")
            {
                // Destroy bullet if the shield is its complement
                if (Mathf.Abs(oh - h) == 180)
                {
                    Destroy(gameObject);
                }
                else
                {
                    // Ensure red is using the correct angle
                    if ((h == 0 && oh > 180) || (h == 360 && oh < 180))
                    {
                        h = Mathf.Abs(h - 360);
                    }
                    else if ((oh == 0 && h > 180) || (oh == 360 && h < 180))
                    {
                        oh = Mathf.Abs(oh - 360);
                    }

                    // Average the two hues to find the bullet's new hue
                    float newHue = (h + oh)/2;
                    sr.color = Color.HSVToRGB(newHue/360, s, v);
                }
            }
        }
        else if (other.gameObject.tag == "Wall")
            Destroy(gameObject);
    }
}
