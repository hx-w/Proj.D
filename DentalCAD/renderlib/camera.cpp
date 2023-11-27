//

#include "renderlib/camera.h"


namespace DentalLib {
	DlCamera::DlCamera() {
		_camera = Camera3D{
			{0.0f, 5.0f, 5.0f}, // position
			{0.0f, 0.0f, 0.0f}, // target
			{0.0f, 1.0f, 0.0f}, // up
			45.0f,              // fov
			CAMERA_PERSPECTIVE
		};
	}

	Vector3& DlCamera::get_pos() {
		return _camera.position;
	}

	void DlCamera::update() {
		/// [TODO] 需要重写update, 使用UpdateCameraPro接口
		UpdateCamera(&_camera, CAMERA_ORBITAL);
	}

	void DlCamera::begin_draw() {
		BeginMode3D(_camera);
	}

	void DlCamera::end_draw() {
		EndMode3D();
	}

}