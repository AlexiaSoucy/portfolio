using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HueSwitcher : MonoBehaviour
{
    // Display variables
    public float hue;
    public SpriteRenderer sr;
    public SpriteRenderer glyph;
    public Sprite glyphR;
    public Sprite glyphG;
    public Sprite glyphB;
    public Sprite glyphC;
    public Sprite glyphM;
    public Sprite glyphY;

    // Start is called before the first frame update
    void Start()
    {
        // Set hue
        sr.color = Color.HSVToRGB(hue/360, 1, 1);

        // Set gylph
        switch (hue)
        {
            case 60:
                glyph.sprite = glyphY;
                break;
            case 120:
                glyph.sprite = glyphG;
                break;
            case 180:
                glyph.sprite = glyphC;
                break;
            case 240:
                glyph.sprite = glyphB;
                break;
            case 300:
                glyph.sprite = glyphM;
                break;
            default:
                glyph.sprite = glyphR;
                break;
        }
    }
}
