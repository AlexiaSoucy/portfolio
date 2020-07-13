using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ButtonControl : MonoBehaviour
{
    public void ChangeScene(string scene)
    {
        SceneManager.LoadScene(scene);
    }

    public void Hide(GameObject hide)
    {
        hide.SetActive(false);
    }

    public void Show(GameObject show)
    {
        show.SetActive(true);
    }

    public void Quit()
    {
        Application.Quit();
    }
}
