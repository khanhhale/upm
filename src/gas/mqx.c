/*
 * Author:
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <stdlib.h>

#include "mqx.h"

mqx_context mqx_init(int16_t pin)
{
    mqx_context dev = (mqx_context) malloc(sizeof(struct _mqx_context));

    if (dev == NULL)
      return NULL;

    /* Init aio pin */
    dev->aio = mraa_aio_init(pin);

    if (dev->aio == NULL) {
        free(dev);
        return NULL;
    }

    /* Set the ADC ref, scale, and offset defaults */
    dev->m_aRef = 5.0;
    dev->m_scale = 1.0;
    dev->m_offset = 0.0;

    return dev;
}

void mqx_close(mqx_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t mqx_set_aref(const mqx_context dev, float aref)
{
    dev->m_aRef = aref;
    return UPM_SUCCESS;
}

upm_result_t mqx_set_scale(const mqx_context dev, float scale)
{
    dev->m_scale = scale;
    return UPM_SUCCESS;
}

upm_result_t mqx_set_offset(const mqx_context dev, float offset)
{
    dev->m_offset = offset;
    return UPM_SUCCESS;
}

float mqx_get_aref(const mqx_context dev)
{
    return dev->m_aRef;
}

float mqx_get_scale(const mqx_context dev)
{
    return dev->m_scale;
}

float mqx_get_offset(const mqx_context dev)
{
    return dev->m_offset;
}

upm_result_t mqx_get_normalized(const mqx_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;
}

upm_result_t mqx_get_raw_volts(const mqx_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;

    /* Scale by the ADC reference voltage */
    *value *= dev->m_aRef;

    return UPM_SUCCESS;
}

upm_result_t mqx_get_volts(const mqx_context dev, float *value)
{
    *value = mraa_aio_read_float(dev->aio);
    if (*value < 0)
        return UPM_ERROR_OPERATION_FAILED;

     /* Apply raw scale */
    *value *= dev->m_scale;

     /* Scale to aRef */
    *value *= dev->m_aRef;

    /* Apply the offset in volts */
    *value += dev->m_offset;

    return UPM_SUCCESS;
}
