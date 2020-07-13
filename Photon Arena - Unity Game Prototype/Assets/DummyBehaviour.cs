using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DummyBehaviour : MonoBehaviour
{
    // Display variables
    public float hue;
    public SpriteRenderer sr;

    // Particle Systems
    public GameObject deathAnim;

    // Start is called before the first frame update
    void Start()
    {
        sr.color = Color.HSVToRGB(hue/360, 1, 1);
    }

    private void OnDestroy() {
        // Create a death animation at this location
        GameObject bullet = Instantiate(deathAnim, transform.position, transform.rotation);
    }
}
