#pragma once

#include "vec3_t.h"

namespace Construct
{
	/// <summary>
	/// Enum for GeneratorSetting that defines face direction.
	/// CCW by default.
	/// </summary>
	enum class FaceDirection : std::uint8_t { CCW, CW };
	/// <summary>
	/// Defines generator settings to generate specific data or data manipulations
	/// </summary>
	struct GeneratorSetting
	{
		vec3_t<float> offset;
		vec3_t<float> scale;
		bool flipVertexY;
		FaceDirection faceDirection;

		/// <summary>
		/// Define generator settings
		/// </summary>
		/// <param name="fvy">Should the generator flip the model (should it be upside down)? false by default</param>
		/// <param name="direction">Defines what direction the generated face will be, CCW by default</param>
		/// <param name="off">Offset vector for the center of models, defaults to 0.0f, 0.0f, 0.0f</param>
		/// <param name="sc">Scale vector for the size of models, defaults to 1.0f, 1.0f, 1.0f</param>
		inline GeneratorSetting(bool fvy = false, FaceDirection direction = FaceDirection::CCW, const vec3_t<float>& off = vec3_t<float>(0.0f, 0.0f, 0.0f), const vec3_t<float>& sc = vec3_t<float>(1.0f, 1.0f, 1.0f))
		{
			this->flipVertexY = fvy;
			this->faceDirection = direction;
			this->offset = off;
			this->scale = sc;
		}
	};
}