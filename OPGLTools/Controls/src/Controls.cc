#include "Controls.hh"
#include "Trigonometry.hh"

#include <GLFW/glfw3.h>
//extern GLFWwindow* window;

Controls *Controls::fInstance = nullptr;

Controls *Controls::GetInstance() {
    if(!fInstance)
        fInstance = new Controls();
    return fInstance;
}

Controls::Controls(){
    // Initial position : on +Z
    fPosition = Vector3( 0, 0, 5 ); 
    // Initial horizontal angle : toward -Z
    fHorizontalAngle = PI;
    // Initial vertical angle : none
    fVerticalAngle = 0.0f;
    // Initial Field of View
    fInitialFoV = 45.0f;

    fSpeed = 3.0f; // 3 units / second
    fMouseSpeed = 0.005f;
}

Controls::~Controls(){
    
}

void Controls::computeMatricesFromInputs(){

    static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

    // Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(fWindow, &xpos, &ypos);

    // Reset mouse position for next frame
	//glfwSetCursorPos(fWindow, 1024/2, 768/2);

	if(fEnabledCursor){
		// Compute new orientation
		fHorizontalAngle += fMouseSpeed * float(1024/2 - xpos );
		fVerticalAngle   += fMouseSpeed * float( 768/2 - ypos );
	}

    // Direction : Spherical coordinates to Cartesian coordinates conversion
	Vector3 direction(
		cos(fVerticalAngle) * sin(fHorizontalAngle), 
		sin(fVerticalAngle),
		cos(fVerticalAngle) * cos(fHorizontalAngle)
	);

    // Right vector
	Vector3 right = Vector3(
		sin(fHorizontalAngle - 3.14f/2.0f), 
		0,
		cos(fHorizontalAngle - 3.14f/2.0f)
	);

    // Up vector
	Vector3 up = right.cross(direction);
    
    // Move forward
	if (glfwGetKey(fWindow, GLFW_KEY_UP ) == GLFW_PRESS && fEnabledUpDown){
		fPosition += direction * deltaTime * fSpeed;
	}
	// Move backward
	if (glfwGetKey( fWindow, GLFW_KEY_DOWN ) == GLFW_PRESS && fEnabledUpDown){
		fPosition -= direction * deltaTime * fSpeed;
	}
	// Strafe right
	if (glfwGetKey( fWindow, GLFW_KEY_RIGHT ) == GLFW_PRESS && fEnabledLeftRight){
		fPosition += right * deltaTime * fSpeed;
	}
	// Strafe left
	if (glfwGetKey( fWindow, GLFW_KEY_LEFT ) == GLFW_PRESS && fEnabledLeftRight){
		fPosition -= right * deltaTime * fSpeed;
	}

    float FoV = fInitialFoV;

    fProjectionMatrix = Perspective(Radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera Matrix
    fViewMatrix       = LookAt(
								fPosition,           // Camera is here
								fPosition+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );


    // For the next frame, the "last time" will be "now"
	lastTime = currentTime;// For the next frame, the "last time" will be "now"
}