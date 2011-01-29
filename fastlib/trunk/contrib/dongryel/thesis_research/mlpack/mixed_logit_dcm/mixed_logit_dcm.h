/** @file mixed_logit_dcm.h
 *
 *  The simulation-based mixed logit discrete choice model class.
 *
 *  @author Dongryeol Lee (dongryel@cc.gatech.edu)
 */

#ifndef MLPACK_MIXED_LOGIT_MIXED_LOGIT_DCM_H
#define MLPACK_MIXED_LOGIT_MIXED_LOGIT_DCM_H

#include <vector>
#include <boost/program_options.hpp>
#include "core/monte_carlo/mean_variance_pair.h"
#include "core/table/table.h"
#include "mlpack/mixed_logit_dcm/dcm_table.h"
#include "mlpack/mixed_logit_dcm/mixed_logit_dcm_arguments.h"
#include "mlpack/mixed_logit_dcm/mixed_logit_dcm_result.h"

namespace mlpack {
namespace mixed_logit_dcm {

/** @brief The definition of mixed logit discrete choice model using
 *         simulation-based approach.
 */
template<typename IncomingTableType>
class MixedLogitDCM {
  public:

    /** @brief The table type being used in the algorithm.
     */
    typedef IncomingTableType TableType;

    /** @brief The discrete choice model table type.
     */
    typedef mlpack::mixed_logit_dcm::DCMTable<TableType> DCMTableType;

    /** @brief The sample type.
     */
    typedef
    mlpack::mixed_logit_dcm::MixedLogitDCMSampling<DCMTableType> SamplingType;

    /** @brief The argument type.
     */
    typedef mlpack::mixed_logit_dcm::MixedLogitDCMArguments <
    TableType > ArgumentType;

  private:

    /** @brief Construct variable map for the algorithm.
     */
    static bool ConstructBoostVariableMap_(
      const std::vector<std::string> &args,
      boost::program_options::variables_map *vm);

    /** @brief Update the sample allocation.
     */
    void UpdateSampleAllocation_(
      const ArgumentType &arguments_in,
      double integration_sample_error,
      const SamplingType &second_sample,
      SamplingType *first_sample) const;

    /** @brief Implements the stopping condition.
     */
    bool TerminationConditionReached_(
      const ArgumentType &arguments_in,
      double model_reduction_ratio,
      double data_sample_error,
      double integration_sample_error,
      const SamplingType &first_sample,
      const arma::vec &gradient) const;

    /** @brief Computes the sample data error (Section 3.1).
     */
    double DataSampleError_(
      const SamplingType &first_sample,
      const SamplingType &second_sample) const;

    void IntegrationSampleErrorPerPerson_(
      int person_index,
      const SamplingType &first_sample,
      const SamplingType &second_sample,
      core::monte_carlo::MeanVariancePair *integration_sample_error) const;

    /** @brief Computes the simulation error (Section 3.2).
     */
    double IntegrationSampleError_(
      const SamplingType &first_sample,
      const SamplingType &second_sample) const;

    /** @brief Computes the gradient error (Section 3.3).
     */
    double GradientError_(const SamplingType &sample) const;

    /** @brief Computes the first part of the gradient error (Section
     *         3.3).
     */
    double GradientErrorFirstPart_(const SamplingType &sample) const;

    /** @brief Computes the second part of the gradient error (Section
     *         3.3).
     */
    double GradientErrorSecondPart_(const SamplingType &sample) const;

  public:

    /** @brief Returns the table holding the discrete choice model
     *         information.
     */
    TableType *attribute_table();

    /** @brief Initializes the mixed logit discrete choice model
     *         object with a set of arguments.
     */
    void Init(ArgumentType &arguments_in);

    /** @brief Computes the result.
     */
    void Compute(
      const ArgumentType &arguments_in,
      mlpack::mixed_logit_dcm::MixedLogitDCMResult *result_out);

    /** @brief Parse the arguments.
     */
    static void ParseArguments(
      const std::vector<std::string> &args,
      ArgumentType *arguments_out);

    /** @brief Parse the arguments.
     */
    static void ParseArguments(
      int argc,
      char *argv[],
      ArgumentType *arguments_out);

  private:

    /** @brief The table that holds the discrete choice model
     *         information.
     */
    DCMTableType table_;
};
}
}

#endif
