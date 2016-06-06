using UnityEngine;
using System.Collections;

public class Camera_Controller : MonoBehaviour {

	public GameObject player;

	private Vector3 offset;

	// Use this for initialization
	void Start () {
		offset = transform.position - player.transform.position;
	}
	
	// Update is called once per frame after everything else
	void LateUpdate () {
		transform.position = player.transform.position + offset;
	}
}
