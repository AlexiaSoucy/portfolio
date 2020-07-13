using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShieldBehaviour : MonoBehaviour
{
    public GameObject parent;

    private void OnDestroy() {
        Destroy(parent);
    }
}
