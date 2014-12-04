#include "Camera.h"

Camera::Camera()
{	
}

Camera::~Camera()
{
}

void Camera::camOut()
{
	float temp_x = eyePosition.x + eye_move * cos(totalRotation.y * PI / 180.0);
	float temp_z = eyePosition.z - eye_move * sin(totalRotation.y * PI / 180.0);

	if (abs(temp_x) < MAX_MOVE && abs(temp_z) < MAX_MOVE)
	{
		mat4 translate = glm::translate(glm::mat4(), vec3(0, 0, -eye_move));
		view = translate * view;
		eyePosition = vec3(temp_x, eyePosition.y, temp_z);
	}
}

void Camera::camIn()
{
	float temp_x = eyePosition.x - eye_move * cos(totalRotation.y * PI / 180.0);
	float temp_z = eyePosition.z + eye_move * sin(totalRotation.y * PI / 180.0);

	if (abs(temp_x) < MAX_MOVE && abs(temp_z) < MAX_MOVE)
	{
		mat4 translate = glm::translate(glm::mat4(), vec3(0, 0, eye_move));
		view = translate * view;
		eyePosition = vec3(temp_x, eyePosition.y, temp_z);
	}
}

void Camera::camUp()
{
	mat4 translate = glm::translate(glm::mat4(), vec3(0, -eye_move, 0));
	view = translate * view;
	eyePosition.y += eye_move;
}

void Camera::camDown()
{
	mat4 translate = glm::translate(glm::mat4(), vec3(0, eye_move, 0));
	view = translate * view;
	eyePosition.y -= eye_move;
}

void Camera::camLeft()
{
	float temp_x = eyePosition.x - eye_move * sin(totalRotation.y * PI / 180.0);
	float temp_z = eyePosition.z - eye_move * cos(totalRotation.y * PI / 180.0);

	if (abs(temp_x) < MAX_MOVE && abs(temp_z) < MAX_MOVE)
	{
		mat4 translate = glm::translate(glm::mat4(), vec3(eye_move, 0, 0));
		view = translate * view;
		eyePosition = vec3(temp_x, eyePosition.y, temp_z);
	}
}

void Camera::camRight()
{
	float temp_x = eyePosition.x + eye_move * sin(totalRotation.y * PI / 180.0);
	float temp_z = eyePosition.z + eye_move * cos(totalRotation.y * PI / 180.0);

	if (abs(temp_x) < MAX_MOVE && abs(temp_z) < MAX_MOVE)
	{
		mat4 translate = glm::translate(glm::mat4(), vec3(-eye_move, 0, 0));
		view = translate * view;
		eyePosition = vec3(temp_x, eyePosition.y, temp_z);
	}
}

// rotate the view
void Camera::camLookLeft()
{
	mat4 rotate = glm::rotate(glm::mat4(), -look_move, vec3((float)0, (float)1, (float)0));
	view = rotate * view;
	totalRotation.y += -look_move;
	updateEyeDirection(glm::inverse(rotate));
}
void Camera::camLookRight()
{
	mat4 rotate = glm::rotate(glm::mat4(), look_move, vec3((float)0, (float)1, (float)0));
	view = rotate * view;
	totalRotation.y += look_move;
	updateEyeDirection(glm::inverse(rotate));
}

void Camera::camLookUp()
{
	mat4 rotate = glm::rotate(glm::mat4(), -look_move, vec3((float)1, (float)0, (float)0));
	rotateMatrix = rotate * rotateMatrix;
	updateEyeDirection(glm::inverse(rotate));
}

void Camera::camLookDown()
{
	mat4 rotate = glm::rotate(glm::mat4(), look_move, vec3((float)1, (float)0, (float)0));
	rotateMatrix = rotate * rotateMatrix;
	updateEyeDirection(glm::inverse(rotate));
}

void Camera::setEyeMove(float move)
{
	eye_move = move;
}

void Camera::setLookAtMove(float move)
{
	look_move = move;
}

float Camera::getEyeX()
{
	return eyePosition.x;
}

float Camera::getEyeY()
{
	return eyePosition.y;
}

float Camera::getEyeZ()
{
	return eyePosition.z;
}

void Camera::init(float x, float y, float z)
{
	eye_move = EYE_MOVE_DEFAULT;
	look_move = LOOK_MOVE_DEFAULT;

	totalRotation = vec3(0.0,90.0,0.0);
	eyePosition = vec3(x, y, z);
	eyeDirection = vec3(0, 0, -1);

	mat4 translate = glm::translate(glm::mat4(), -eyePosition);
	rotateMatrix = glm::mat4();
	view = translate;
	frustum = glm::frustum((float)-0.2, (float)0.2, (float)-0.2, (float)0.2, (float)0.3, (float)10000);
}

void Camera::updateEyeDirection(mat4 inMat)
{
	vec4 temp = vec4(eyeDirection, 1.0);
	temp = inMat * temp;
	eyeDirection = vec3(temp);
}

vec3 Camera::getEyeDirection()
{
	return eyeDirection;
}

vec3 Camera::getEyePosition()
{
	return eyePosition;
}

vec3 Camera::getTotalRotation()
{
	return totalRotation;
}

void Camera::reverse()
{
	
}

// Sets up uniform for the View Projection Matrix
void Camera::render(Shader shader)
{
	glUniformMatrix4fv(shader.getUniformLocation("VPMatrix"), 1, GL_FALSE, glm::value_ptr(frustum * rotateMatrix * view));
	glUniformMatrix4fv(shader.getUniformLocation("VMatrix"), 1, GL_FALSE, glm::value_ptr(rotateMatrix * view));
}

void Camera::setFrustumBack(float back)
{
	frustum = glm::frustum((float)-0.2, (float)0.2, (float)-0.2, (float)0.2, (float)0.3, (float)back);
}

void Camera::updateTarget()
{
	vec4 tempVec = vec4(look_x, look_y, look_z, 1.0);

	look_x = tempVec.x;
	look_y = tempVec.y;
	look_z = tempVec.z;

	cout << "Target: ( " << look_x << " , " << look_y << " , " << look_z << " )\n";
}

float Camera::getLookX()
{
	return look_x;
}

float Camera::getLookY()
{
	return look_y;
}

float Camera::getLookZ()
{
	return look_z;
}

mat4 Camera::getView()
{
	return view;
}

void Camera::setView(mat4 inView)
{
	view = inView;
}