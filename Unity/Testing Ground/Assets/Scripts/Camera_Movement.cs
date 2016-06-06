using UnityEngine;
using System.Collections;

public class Camera_Movement : MonoBehaviour {

	public GameObject player;
	public enum RotationAxes { MouseXAndY = 0, MouseX = 1, MouseY = 2 }
	public RotationAxes axes = RotationAxes.MouseXAndY;
	public float sensitivityX = 15F;
	public float sensitivityY = 15F;
	public float minimumX = -360F;
	public float maximumX = 360F;
	public float minimumY = -89F;
	public float maximumY = 89F;
	float rotationY = 0F;

	private Vector3 cameraPos;
	private Rigidbody player_rigidbody;

	void Start () {
		cameraPos = transform.position - player.transform.position;
		player_rigidbody = player.GetComponent<Rigidbody> ();
	}

	void Update () {
		if (axes == RotationAxes.MouseXAndY) {
			float rotationX = transform.localEulerAngles.y + Input.GetAxis("MouseX") * sensitivityX;

			rotationY += Input.GetAxis("MouseY") * sensitivityY;
			rotationY = Mathf.Clamp (rotationY, minimumY, maximumY);

			transform.localEulerAngles = new Vector3(-rotationY, rotationX, 0);
			player_rigidbody.transform.localEulerAngles = new Vector3(0, rotationX, 0);
		}
		else if (axes == RotationAxes.MouseX) {
			transform.Rotate(0, Input.GetAxis("MouseX") * sensitivityX, 0);
		}
		else {
			rotationY += Input.GetAxis("MouseY") * sensitivityY;
			rotationY = Mathf.Clamp (rotationY, minimumY, maximumY);

			transform.localEulerAngles = new Vector3(-rotationY, transform.localEulerAngles.y, 0);
		}
	}

	void LateUpdate () {
		transform.position = player.transform.position + cameraPos;
	}
}
