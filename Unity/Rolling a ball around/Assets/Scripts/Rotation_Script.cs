using UnityEngine;
using System.Collections;

public class Rotation_Script : MonoBehaviour {

	public Vector3 speed;
	
	// Update is called once per frame
	void Update () {
		transform.Rotate (speed * Time.deltaTime); // BY MAKING STATIC OBJECTS THAT MOVE KINEMATIC RIGID BODIES UNITY DOOESNT CACHE THEIR INFO ON TRANSFORM AND RUNS BETTER
	}
}
