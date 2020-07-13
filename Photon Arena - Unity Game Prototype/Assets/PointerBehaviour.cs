using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PointerBehaviour : MonoBehaviour
{
    private Vector3 target = new Vector3(0f, 0f, 0f);

    // Update is called once per frame
    void Update()
    {
        // Find normalized direction from player to target
        Vector2 direction = (Vector2)transform.position - (Vector2)target;
        direction.Normalize();

        // Find angle between player and enemy with cross product
        float rotation = Vector3.Cross(direction, transform.up).z;

        // Rotate pointer
        transform.Rotate(new Vector3(0f, 0f, 1f), rotation);
    }

    public void SetTarget(Vector3 t)
    {
        target = t;
    }

    public void Die()
    {
        Destroy(gameObject);
    }
}
