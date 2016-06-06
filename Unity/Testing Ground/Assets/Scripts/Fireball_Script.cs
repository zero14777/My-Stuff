using UnityEngine;
using System.Collections;

public class Fireball_Script : MonoBehaviour {
	public float duration = 5.0f;
	public GameObject fireball;

	private float count = 0.0f;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		count += Time.deltaTime;
		if (count >= duration)
		{
			Destroy (fireball);
		}
	}
}
