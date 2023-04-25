#pragma once

#include "types.hpp"

namespace Construct
{
	/// <summary>
	/// Enum for GeneratorSetting that defines winding order.
	/// CCW by default.
	/// </summary>
	enum class WindingOrder : std::uint8_t { CCW, CW };
	/// <summary>
	/// Defines generator settings to generate specific data or data manipulations
	/// </summary>
	struct GeneratorSetting
	{
		vec3 offset;
		vec3 scale;
		quat rotation;
		WindingOrder windingOrder;
		/// <summary>
		/// Define generator settings
		/// </summary>
		/// <param name="windingOrder">Defines what winding order the generated face will have, CCW by default</param>
		/// <param name="off">Offset vector for the center of models, defaults to 0.0f, 0.0f, 0.0f</param>
		/// <param name="sc">Scale vector for the size of models, defaults to 1.0f, 1.0f, 1.0f</param>
		inline GeneratorSetting(WindingOrder windingOrder = WindingOrder::CCW, const vec3& off = vec3(0.0f, 0.0f, 0.0f), const vec3& sc = vec3(1.0f, 1.0f, 1.0f), const quat& qu = quat(0.0f, 0.0f, 0.0f, 1.0f))
		{
			this->windingOrder = windingOrder;
			this->offset = off;
			this->scale = sc;
			this->rotation = qu;
		}
	};
}