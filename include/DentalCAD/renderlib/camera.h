//
// Created by carol on 2023/11/27.
//

#ifndef DENTALCAD_CAMERA_H
#define DENTALCAD_CAMERA_H

#include <raylib.h>

namespace DentalLib {
	class DlCamera {
	public:
		DlCamera();
		~DlCamera() {};

		Vector3& get_pos();

		void update();

		void begin_draw();

		void end_draw();

	private:
		Camera _camera;
	};
}

#endif
