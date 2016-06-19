using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System;

//My first attempt at creating a first person control setup
//Rotation is handled by the Camera_Movment script

public class fps_player_controls : MonoBehaviour {
	public float ground_accel;
	public float air_drift;
	public float hops;
	public float max_speed;
	public GameObject player;
	public GameObject feet;
	public Text display_input;
	public Camera player_camera;
	public GameObject fireball;

	private bool grounded, walking, stunned, grappling;
	private Vector3 footbox, grappling_hook;
	private Rigidbody player_rigidbody;
	private AudioSource jump, land, walk;
	private Vector3 move;

	void Start() {
		player_rigidbody = player.GetComponent<Rigidbody>();
		AudioSource[] sounds = player.GetComponents<AudioSource>(); //currently using standard assets
		jump = sounds [0];
		land = sounds [1];
		walk = sounds [2];

		Cursor.visible = false;
		grounded = false;
		player_rigidbody.drag = 10.0f;
		footbox = new Vector3 (0.245f, 0.05f, 0.245f);
	}

	void Update() {
		input ();
		//Debug.Log (player_rigidbody.velocity.magnitude);
	}

	void FixedUpdate(){
		if (!stunned && !grappling) {
			movement ();
		} else if (grappling) {
			move = grappling_hook - player_rigidbody.position;
			if (move.magnitude < 1) {
				grappling = false;
			}
			move.Normalize ();
			player_rigidbody.AddForce (move * max_speed, ForceMode.Impulse);
		}
	}

	//This method is meant to handle all input apart from input related to movement
	void input () {
		// An example of instantiating new objects. In this case a fireball that gets shot out in the direction the player is looking
		if (Input.GetKeyDown ("f")) {
			GameObject fireball_inst = Instantiate (fireball, player_camera.transform.position, player_camera.transform.rotation) as GameObject;
			fireball_inst.GetComponent<Rigidbody> ().AddForce (player_camera.transform.forward * 2000);
		}
		// An example of raycasting and collecting information on the first object hit. By using hit.collider.gameObject you could
		// cause the object hit to react however desired.
		if (Input.GetKeyDown ("g")) {
			RaycastHit hit;
			if (Physics.Raycast (player_camera.transform.position, player_camera.transform.forward, out hit, 100.0f)) {
				display_input.text = "Hit " + hit.collider.name + " at distance " + hit.distance.ToString();
				//Debug.DrawRay (player_camera.transform.position, player_camera.transform.forward);
			}
		}
		// This is a display of using raycasting to implement a simple grappling hook
		if (Input.GetKeyDown ("h")) {
			RaycastHit hit;
			if (Physics.Raycast (player_camera.transform.position, player_camera.transform.forward, out hit, 100.0f)) {
				grappling = true;
				grappling_hook = hit.point;
			}
		}
	}

	//This method handles the movement input and should be called in the FixedUpdate methond
	void movement () {
		move = new Vector3 (0, 0, 0);
		if (Physics.CheckBox (feet.transform.position, footbox, Quaternion.identity, LayerMask.GetMask ("Default"))) {
			if (!grounded) { //play sound on landing{
				land.Play ();
			}

			grounded = true; 
			player_rigidbody.drag = 10.0f;

			if (Input.GetKeyDown ("space")) { //jumping action
				player_rigidbody.AddForce (0, hops, 0, ForceMode.Impulse);
				jump.Play();
			}

			grab_wasd ();

			move.Normalize ();
			if (player_rigidbody.velocity.magnitude <= max_speed) {
				player_rigidbody.AddForce (move * ground_accel, ForceMode.Impulse);
			}
			if (move.magnitude > 0 && !walk.isPlaying) {
				walk.Play ();
			}
			if (move.magnitude == 0 && walk.isPlaying) {
				walk.Stop ();
			}
		}
		else {
			walk.Stop (); // dont play walking sound midair
			grounded = false;
			player_rigidbody.drag = 0.0f;

			grab_wasd ();

			move.Normalize ();
			if (player_rigidbody.velocity.magnitude <= max_speed) {
				player_rigidbody.AddForce (move * air_drift, ForceMode.Impulse);
			}
		}
	}

	void grab_wasd () {
		move.x = 0;
		move.y = 0;
		move.z = 0;
		if (Input.GetKey ("w")) {
			move.x += player_camera.transform.forward.x;
			move.z += player_camera.transform.forward.z;
		}
		if (Input.GetKey ("s")) {
			move.x += -player_camera.transform.forward.x;
			move.z += -player_camera.transform.forward.z;
		}
		if (Input.GetKey ("a")) {
			move.x += -player_camera.transform.right.x;
			move.z += -player_camera.transform.right.z;
		}
		if (Input.GetKey ("d")) {
			move.x += player_camera.transform.right.x;
			move.z += player_camera.transform.right.z;
		}
	}
}