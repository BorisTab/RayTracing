target_sources(${TARGET_NAME} PUBLIC
                ${INCLUDE_ROOT}/geometry.h ${INCLUDE_ROOT}/material.h ${INCLUDE_ROOT}/cuda_error_checker.h
                ${INCLUDE_ROOT}/double_equal.h double_equal.cu
                ${INCLUDE_ROOT}/canvas.h canvas.cu
                ${INCLUDE_ROOT}/sphere.h sphere.cu
                ${INCLUDE_ROOT}/scene.h scene.cu
                ${INCLUDE_ROOT}/cuda_ray_tracing.h cuda_ray_tracing.cu)
