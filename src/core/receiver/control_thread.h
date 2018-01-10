/*!
 * \file control_thread.h
 * \brief Interface of the receiver control plane
 * \author Carlos Aviles, 2010. carlos.avilesr(at)googlemail.com
 *
 * GNSS Receiver Control Plane: connects the flowgraph, starts running it,
 * and while it does not stop, reads the control messages generated by the blocks,
 * processes them, and applies the corresponding actions.
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2015  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

#ifndef GNSS_SDR_CONTROL_THREAD_H_
#define GNSS_SDR_CONTROL_THREAD_H_

#include <memory>
#include <vector>
#include <boost/thread.hpp>
#include <gnuradio/msg_queue.h>
#include "control_message_factory.h"
#include "gnss_sdr_supl_client.h"

class GNSSFlowgraph;
class ConfigurationInterface;


/*!
 * \brief This class represents the main thread of the application, so the name is ControlThread.
 * This is the GNSS Receiver Control Plane: it connects the flowgraph, starts running it,
 * and while it does not stop, reads the control messages generated by the blocks,
 * processes them, and applies the corresponding actions.
 */
class ControlThread
{
public:
    /*!
     * \brief Default constructor
     */
    ControlThread();

    /*!
     * \brief Constructor that initializes the class with parameters
     *
     * \param[in] configuration Pointer to a ConfigurationInterface
     */
    ControlThread(std::shared_ptr<ConfigurationInterface> configuration);

    //! \brief Destructor
    ~ControlThread();

    /*! \brief Runs the control thread
     *
     *  This is the main loop that reads and process the control messages:
     *
     *  - Connect the GNSS receiver flowgraph;
     *
     *  - Start the GNSS receiver flowgraph;
     *
     *  while (flowgraph_->running() && !stop_){
     *
     *  - Read control messages and process them; }
     */
    void run();

    /*!
     * \brief Sets the control_queue
     *
     * \param[in] boost::shared_ptr<gr::msg_queue> control_queue
     */
    void set_control_queue(gr::msg_queue::sptr control_queue);


    unsigned int processed_control_messages()
    {
        return processed_control_messages_;
    }

    unsigned int applied_actions()
    {
        return applied_actions_;
    }

    /*!
     * \brief Instantiates a flowgraph
     *
     * \return Returns a smart pointer to a flowgraph object
     */
    std::shared_ptr<GNSSFlowgraph> flowgraph()
    {
        return flowgraph_;
    }

private:
    //SUPL assistance classes
    gnss_sdr_supl_client supl_client_acquisition_;
    gnss_sdr_supl_client supl_client_ephemeris_;
    int supl_mcc; // Current network MCC (Mobile country code), 3 digits.
    int supl_mns; // Current network MNC (Mobile Network code), 2 or 3 digits.
    int supl_lac; // Current network LAC (Location area code),16 bits, 1-65520 are valid values.
    int supl_ci;  // Cell Identity (16 bits, 0-65535 are valid values).

    void init();

    // Read {ephemeris, iono, utc, ref loc, ref time} assistance from a local XML file previously recorded
    bool read_assistance_from_XML();

    // Save {ephemeris, iono, utc, ref loc, ref time} assistance to a local XML file
    //bool save_assistance_to_XML();

    void read_control_messages();

    void process_control_messages();

    /*
     * Blocking function that reads the GPS assistance queue
     */
    void gps_acq_assist_data_collector();
    
    /*
     * Read initial GNSS assistance from SUPL server or local XML files
     */
    void assist_GNSS();
    
    void apply_action(unsigned int what);
    std::shared_ptr<GNSSFlowgraph> flowgraph_;
    std::shared_ptr<ConfigurationInterface> configuration_;
    gr::msg_queue::sptr control_queue_;
    std::shared_ptr<ControlMessageFactory> control_message_factory_;
    std::shared_ptr<std::vector<std::shared_ptr<ControlMessage>>> control_messages_;
    bool stop_;
    bool delete_configuration_;
    unsigned int processed_control_messages_;
    unsigned int applied_actions_;
    boost::thread keyboard_thread_;
    boost::thread sysv_queue_thread_;
    boost::thread gps_acq_assist_data_collector_thread_;
    
    void keyboard_listener();
    void sysv_queue_listener();
    int msqid;

    // default filename for assistance data
    const std::string eph_default_xml_filename = "./gps_ephemeris.xml";
    const std::string utc_default_xml_filename = "./gps_utc_model.xml";
    const std::string iono_default_xml_filename = "./gps_iono.xml";
    const std::string ref_time_default_xml_filename = "./gps_ref_time.xml";
    const std::string ref_location_default_xml_filename = "./gps_ref_location.xml";
};

#endif /*GNSS_SDR_CONTROL_THREAD_H_*/
