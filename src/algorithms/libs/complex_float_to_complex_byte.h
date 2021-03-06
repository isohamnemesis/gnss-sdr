/*!
 * \file complex_float_to_complex_byte.h
 * \brief Adapts a gr_complex stream into a std::complex<signed char> stream
 * \author Carles Fernandez Prades, cfernandez(at)cttc.es
 *
 *
 * -----------------------------------------------------------------------------
 *
 * GNSS-SDR is a Global Navigation Satellite System software-defined receiver.
 * This file is part of GNSS-SDR.
 *
 * Copyright (C) 2010-2020  (see AUTHORS file for a list of contributors)
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------------
 */

#ifndef GNSS_SDR_COMPLEX_FLOAT_TO_COMPLEX_BYTE_H
#define GNSS_SDR_COMPLEX_FLOAT_TO_COMPLEX_BYTE_H

#include "gnss_block_interface.h"
#include <gnuradio/sync_block.h>
#include <gnuradio/types.h>  // for gr_vector_const_void_star

/** \addtogroup Algorithms_Library
 * \{ */
/** \addtogroup Algorithm_libs algorithms_libs
 * \{ */


class complex_float_to_complex_byte;

using complex_float_to_complex_byte_sptr = gnss_shared_ptr<complex_float_to_complex_byte>;

complex_float_to_complex_byte_sptr make_complex_float_to_complex_byte();

/*!
 * \brief This class adapts a gr_complex stream into a std::complex<signed char> stream
 */
class complex_float_to_complex_byte : public gr::sync_block
{
public:
    int work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items);

private:
    friend complex_float_to_complex_byte_sptr make_complex_float_to_complex_byte();
    complex_float_to_complex_byte();
};


/** \} */
/** \} */
#endif  // GNSS_SDR_COMPLEX_FLOAT_TO_COMPLEX_BYTE_H
