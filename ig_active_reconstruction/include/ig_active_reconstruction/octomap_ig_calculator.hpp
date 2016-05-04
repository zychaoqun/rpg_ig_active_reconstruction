/* Copyright (c) 2015, Stefan Isler, islerstefan@bluewin.ch
*
This file is part of ig_active_reconstruction, a ROS package for...well,

ig_active_reconstruction is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ig_active_reconstruction is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with ig_active_reconstruction. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "ig_active_reconstruction/world_representation_communication_interface.hpp"
#include "ig_active_reconstruction/octomap_world_representation.hpp"

#include "ig_active_reconstruction/factory.hpp"
#include "ig_active_reconstruction/octomap_information_gain.hpp"
#include "ig_active_reconstruction/octomap_map_metric.hpp"

namespace ig_active_reconstruction
{
  
namespace world_representation
{
  
namespace octomap
{  
  /*! Abstract base class: Provides information gain calculation for octomap-based probabilistic volumetric world representation, implementing the frameworks communication interface. Additionally it includes a factory where the desired information gain and 
   * map metric methods can be registered.
   */
  template<class TREE_TYPE>
  class IgCalculator: public CommunicationInterface, public WorldRepresentation<TREE_TYPE>::LinkedObject
  {
  public:
    typedef multikit::Factory< InformationGain<TREE_TYPE> > IgFactory;
    typedef multikit::Factory< MapMetric<TREE_TYPE> > MmFactory;
    
  public:
    virtual ~IgCalculator(){};
    
  // Interface implementation
  public:
    /*! Calculates a set of information gains for a given view.
     * @param command Specifies which information gains have to be calculated and for which pose along with further parameters that define how the ig('s) will be collected.
     * @param output_ig (Output) Vector with the results of the information gain calculation. The indices correspond to the indices of the names in the metric_names array within the passed command.
     */
    virtual ResultInformation computeViewIg(IgRetrievalCommand& command, ViewIgRetrievalResult& output_ig)=0;
    
    /*! Calculates a set of evaluation metrics on the complete map.
     * @param command Specifies which metrics shall be calculated.
     */
    virtual ResultInformation computeMapMetric(MapMetricRetrievalCommand& command, MapMetricRetrievalResultSet& output)=0;
    
    /*! Returns all available information gain metrics.
     * @param available_ig_metrics (output) Set of available metrics.
     */
    virtual void availableIgMetrics( std::vector<MetricInfo>& available_ig_metrics )=0;
    
    /*! Returns all available map metrics.
     * @param available_map_metrics (output) Set of available map metrics.
     */
    virtual void availableMapMetrics( std::vector<MetricInfo>& available_map_metrics )=0;
    
    /*! Registers an information gains with optional constructor parameters that will then be available for
     * calculations.
     * 
     * In order to allow multithreaded calculations, constructor and
     * arguments are saved and a new object is instantiated each time a
     * metric is to be calculated for a new view.
     * 
     * @tparam IG_METRIC_TYPE Type of the information gain that is to be created. It must be templated on the TREE_TYPE, which will be automatically matched to the one of the IgCalculator object.
     * @param args (variadic) Constructor arguments to the created IG_METRIC_TYPE, used for custom configuration.
     * @return The identifier for the object type within the factory.
     */
    template<template<typename> class IG_METRIC_TYPE, typename ... IG_CONSTRUCTOR_ARGS>
    unsigned int registerInformationGain( IG_CONSTRUCTOR_ARGS ... args );
    
  protected:
    IgFactory ig_factory_; //! Information gain factory.
    MmFactory mm_factory_; //! Map metric factory.
  };
}

}

}

#include "../src/code_base/octomap_ig_calculator.inl"