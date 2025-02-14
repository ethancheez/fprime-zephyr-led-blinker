// ======================================================================
// \title  LedBlinkerTopology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <LedBlinker/Top/LedBlinkerTopologyAc.hpp>
#include <LedBlinker/Top/LedBlinkerPacketsAc.hpp>
#include <config/FppConstantsAc.hpp>

// Necessary project-specified types
#include <Svc/FramingProtocol/FprimeProtocol.hpp>

#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec led_pin = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace LedBlinker;

// The reference topology uses the F´ packet protocol when communicating with the ground and therefore uses the F´
// framing and deframing implementations.
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;

// The reference topology divides the incoming clock signal (1Hz) into sub-signals: 1/100Hz, 1/200Hz, and 1/1000Hz
NATIVE_INT_TYPE rateGroupDivisors[Svc::RateGroupDriver::DIVIDER_SIZE] = {100, 200, 1000};

// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
NATIVE_INT_TYPE rateGroup1Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisors, FW_NUM_ARRAY_ELEMENTS(rateGroupDivisors));

    // Rate groups require context arrays.
    rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));

    // Framer and Deframer components need to be passed a protocol handler
    framer.setup(framing);
    deframer.setup(deframing);

    gpioDriver.open(led_pin, Zephyr::ZephyrGpioDriver::GpioDirection::OUT);
}

// Public functions for use in main program are namespaced with deployment name LedBlinker
namespace LedBlinker {
void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Project-specific component configuration. Function provided above. May be inlined, if desired.
    configureTopology();
    // Autocoded parameter loading. Function provided by autocoder.
    // loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);
    
    rateDriver.configure(1);
    commDriver.configure(state.dev, state.uartBaud);
    rateDriver.start();
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);
}
};  // namespace LedBlinker
