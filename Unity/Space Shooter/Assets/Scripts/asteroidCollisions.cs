using UnityEngine;
using System.Collections;

public class asteroidCollisions : MonoBehaviour {

	public GameObject explosion;
	public GameObject playerExplosion;
	public int scoreValue;
	private GameController gameController;

	void Start (){
		GameObject gameControllerObject = GameObject.FindWithTag ("GameController");
		if (gameControllerObject != null) {
			gameController = gameControllerObject.GetComponent<GameController> ();
		} else {
			Debug.Log ("Cannot find Game Controller.");
		}
	}

	// Update is called once per frame
	void OnTriggerEnter(Collider other) {
		//Debug.Log (other.name);
		if (other.tag == "Boundary"){
			return;
		}
		Instantiate (explosion, other.transform.position, other.transform.rotation);
		if (other.tag == "Player") {
			Instantiate (playerExplosion, other.transform.position, other.transform.rotation);
			gameController.GameOver ();
		} else {
			gameController.AddScore (scoreValue);
		}
		Destroy (other.gameObject);
		Destroy (gameObject);
	}
}
