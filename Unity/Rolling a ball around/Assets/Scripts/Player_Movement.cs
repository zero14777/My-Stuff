using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Player_Movement : MonoBehaviour {

	public float speed; // PUBLIC VARIABLES ARE CHANGABLE IN EDITOR
	public Text countText;
	public Text winText;

	private int count;
	private Rigidbody rb;

	void Start () // HAPPENS ON FIRST FRAME WHEN SCRIPT IS ACTIVE
	{
		count = 0;
		rb = GetComponent<Rigidbody> ();
		SetCountText ();
		winText.text = "";
	}

	void FixedUpdate () // CALLED BEFORE ANY PHYSICS CALCULATIONS
	{
		float moveX = Input.GetAxis ("Horizontal");
		float moveY = Input.GetAxis ("Vertical");

		Vector3 movement = new Vector3 (moveX, 0.0f, moveY);

		rb.AddForce (movement*speed);
	}

	void OnTriggerEnter (Collider other)  // SINCE WE USE ON TRIGGER WE NEED TO MAKE ANYTHING WE PLAN TO ACTIVATE THIS A TRIGGER IN ITS COLLIDER COMPONENT
	{
		if (other.gameObject.CompareTag ("Pick Up"))
		{
			other.gameObject.SetActive (false);
			count = count + 1;
			SetCountText ();
		}
	}

	void SetCountText ()
	{
		countText.text = "Count: " + count.ToString ();
		if (count >= 9)
		{
			winText.text = "CONGRATULATION!";
		}
	}
}
