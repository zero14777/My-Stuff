using UnityEngine;
using UnityEngine.SceneManagement;
using System.Collections;

public class GameController : MonoBehaviour {

	public GameObject Hazard;
	public Vector3 spawnValues;
	public int hazardCount;
	public float spawnWait;
	public float startWait;
	public float waveWait;

	public GUIText scoreText;
	public GUIText restartText;
	public GUIText gameoverText;

	private int score;
	private bool gameover;
	private bool restart;

	// Use this for initialization
	void Start () {
		gameover = false;
		restart = false;
		score = 0;
		UpdateScore ();
		StartCoroutine (SpawnWaves ());
	}

	void Update () {
		if (restart) {
			if (Input.GetKeyDown ("r")) {
				string sceneName = SceneManager.GetActiveScene ().name;
				SceneManager.LoadScene (sceneName, LoadSceneMode.Single);
			}
		}
	}

	// Update is called once per frame
	IEnumerator SpawnWaves () {
		while (true) {
			yield return new WaitForSeconds (startWait);
			for (int i = 0; i < hazardCount; i++) {
				Vector3 spawnPosition = new Vector3 (Random.Range (-spawnValues.x, spawnValues.x), spawnValues.y, spawnValues.z);
				Instantiate (Hazard, spawnPosition, Quaternion.identity);
				yield return new WaitForSeconds (spawnWait);
			}
			yield return new WaitForSeconds (waveWait);
			if (gameover) {
				restartText.text = "Press 'R' to Restart";
				restart = true;
				break;
			}
		}
	}

	public void AddScore (int scoreValue) {
		score += scoreValue;
		UpdateScore ();
	}

	void UpdateScore () {
		scoreText.text = "Score: " + score;
	}

	public void GameOver () {
		gameoverText.text = "Game Over!";
		gameover = true;
	}
}
