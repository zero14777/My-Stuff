using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Player_Control : MonoBehaviour {
	public float ground_accel;
	public float air_drift;
	public float hops;
	public float max_speed; // Currently not yet enforced
	public GameObject player;
	public GameObject feet;
	public Text display_input;
	public Camera player_camera;
	public GameObject fireball;

	private bool grounded, walking;
	private Vector3 footbox, sidebox;
	private Rigidbody player_rigidbody;
	private AudioSource jump, land, walk;
	private Vector3 move;

	void Start()  {
		player_rigidbody = player.GetComponent<Rigidbody>();
		AudioSource[] sounds = player.GetComponents<AudioSource>(); //currently using standard assets
		jump = sounds [0];
		land = sounds [1];
		walk = sounds [2];

		Cursor.visible = false;
		grounded = false;
		player_rigidbody.drag = 10.0f;
		footbox = new Vector3 (0.245f, 0.05f, 0.245f);
		sidebox = new Vector3 (0.255f, 0.495f, 0.255f);
	}

	void Update()  {
		check_input ();
		Debug.Log (player_rigidbody.velocity.magnitude);
	}

	void FixedUpdate() {
		movement ();
	}

	void check_input () { // AVOID GETKEY GETBUTTON if possible as only so many can work at a time 
		if (Input.GetKeyDown ("f")) {
			GameObject fireball_inst = Instantiate (fireball, player_camera.transform.position, player_camera.transform.rotation) as GameObject;
			fireball_inst.GetComponent<Rigidbody> ().AddForce (player_camera.transform.forward * 2000);
		}
	}

	void movement () {
		move = new Vector3 (0, 0, 0);
		if (Physics.CheckBox (feet.transform.position, footbox, Quaternion.identity, LayerMask.GetMask ("Default"))) {
			if (!grounded) { //play sound on landing
				land.Play ();
			}

			grounded = true; 
			player_rigidbody.drag = 10.0f;

			if (Input.GetKeyDown ("space")) { //jumping action
				player_rigidbody.AddForce (0, hops, 0, ForceMode.Impulse);
				jump.Play();
			}

			move.x += player_camera.transform.forward.x;
			move.z += player_camera.transform.forward.z;
			move.Normalize ();
			player_rigidbody.AddForce (move*ground_accel, ForceMode.Impulse);

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

			move.x += player_camera.transform.forward.x;
			move.z += player_camera.transform.forward.z;
			move.Normalize ();
			if (Physics.CheckBox (player.transform.position, sidebox, Quaternion.identity, LayerMask.GetMask ("Default"))) {
				player_rigidbody.drag = 5.0f;
				if (Input.GetKeyDown ("space")) { //jumping action
					player_rigidbody.AddForce (0, hops, 0, ForceMode.Impulse);
					jump.Play();
					move.z += 2;
				}
			}
			player_rigidbody.AddForce (move*air_drift, ForceMode.Impulse);
		}
	}
}
