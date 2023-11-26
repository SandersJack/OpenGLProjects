#include "Controls.hh"

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
    fPosition = glm::vec3( 0, 0, 5 ); 
    // Initial horizontal angle : toward -Z
    fHorizontalAngle = 3.14f;
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
	glfwSetCursorPos(fWindow, 1024/2, 768/2);

	if(fEnabledCursor){
		// Compute new orientation
		fHorizontalAngle += fMouseSpeed * float(1024/2 - xpos );
		fVerticalAngle   += fMouseSpeed * float( 768/2 - ypos );
	}

    // Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(fVerticalAngle) * sin(fHorizontalAngle), 
		sin(fVerticalAngle),
		cos(fVerticalAngle) * cos(fHorizontalAngle)
	);

    // Right vector
	glm::vec3 right = glm::vec3(
		sin(fHorizontalAngle - 3.14f/2.0f), 
		0,
		cos(fHorizontalAngle - 3.14f/2.0f)
	);

    // Up vector
	glm::vec3 up = glm::cross( right, direction );
    
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

    fProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera Matrix
    fViewMatrix       = glm::lookAt(
								fPosition,           // Camera is here
								fPosition+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );


    // For the next frame, the "last time" will be "now"
	lastTime = currentTime;// For the next frame, the "last time" will be "now"
}