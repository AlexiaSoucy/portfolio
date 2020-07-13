using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HitAnim : MonoBehaviour
{
    // Particle Systems
    public GameObject hitAnim;

    private void OnDestroy() {
        // Create a hit animation at this location
        GameObject anim = Instantiate(hitAnim, transform.position, transform.rotation);
    }
}
