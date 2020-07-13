using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectorBehaviour : MonoBehaviour
{
    // VFX
    public ParticleSystem selectionPS;
    public ParticleSystem whitePS;

    public void Rotate(float degrees)
    {
        // Rotate selector
        transform.Rotate(0, 0, -degrees);

        // Play selection animation (not displaying properly in webGL; deactivating for web player)
        //selectionPS.Play();
    }

    public void PlayWhite()
    {
        // (not displaying properly in webGL; deactivating for web player)
        //whitePS.Play();
    }

    public void StopWhite()
    {
        // (not displaying properly in webGL; deactivating for web player)
        //whitePS.Stop();
    }
}
