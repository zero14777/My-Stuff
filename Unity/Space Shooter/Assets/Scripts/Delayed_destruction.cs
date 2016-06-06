using UnityEngine;
using System.Collections;

public class Delayed_destruction : MonoBehaviour {

	public float lifetime;

	void Start()
	{
		Destroy (gameObject, lifetime);
	}
}
