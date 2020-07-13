using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DeathAnim : MonoBehaviour
{
    // Particle Systems
    public GameObject deathAnim;

    private void OnDestroy() {
        // Create a death animation at this location
        GameObject anim = Instantiate(deathAnim, transform.position, transform.rotation);

        // Set rotation, then play
        ParticleSystem animPS = anim.GetComponent<ParticleSystem>();
        var main = animPS.main;
        main.startRotationZ = Mathf.Deg2Rad * -transform.rotation.eulerAngles.z;
        animPS.Play();
    }
}
