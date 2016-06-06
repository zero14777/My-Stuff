using UnityEngine;
using System.Collections;

public class Isometric_Camera : MonoBehaviour {

	public GameObject camera_stand;

	private Vector3 cameraPos;

	// Use this for initialization
	void Start () {
	}

	void Update () {
		if (Input.GetKeyDown ("q")) { // do this a smarter way
			//transform.localEulerAngles = new Vector3(transform.localEulerAngles.x, transform.localEulerAngles.y - 45, 0);
			camera_stand.transform.localEulerAngles = new Vector3(0, camera_stand.transform.localEulerAngles.y - 45, 0);
		}
		if (Input.GetKeyDown ("e")) {
			//transform.localEulerAngles = new Vector3(transform.localEulerAngles.x, transform.localEulerAngles.y + 45, 0);
			camera_stand.transform.localEulerAngles = new Vector3(0, camera_stand.transform.localEulerAngles.y + 45, 0);
		}
	}

	// Update is called once per frame
	void LateUpdate () {
	}
}
