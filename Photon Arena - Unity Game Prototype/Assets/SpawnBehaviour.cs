using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnBehaviour : MonoBehaviour
{
    public Transform playerLoc;

    private void FixedUpdate() {
        if (playerLoc != null)
            transform.up = playerLoc.position - transform.position;
    }
}
