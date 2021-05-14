target_sources(${TARGET_NAME} PUBLIC
                ${INCLUDE_ROOT}/cuda_error_checker.h
                ${INCLUDE_ROOT}/material.h material.cu
                ${INCLUDE_ROOT}/geometry.h geometry.cu
                ${INCLUDE_ROOT}/double_equal.h double_equal.cu
                ${INCLUDE_ROOT}/canvas.h canvas.cu
                ${INCLUDE_ROOT}/sphere.h sphere.cu
                ${INCLUDE_ROOT}/scene.h scene.cu
                ${INCLUDE_ROOT}/cuda_ray_tracing.h cuda_ray_tracing.cu)
