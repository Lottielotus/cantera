/**
 *  @file KineticsFactory.h
 */
// Copyright 2001  California Institute of Technology


#ifndef KINETICS_FACTORY_H
#define KINETICS_FACTORY_H

#include "Kinetics.h"
#include "cantera/base/FactoryBase.h"

namespace Cantera
{

class UnknownKineticsModel : public CanteraError
{
public:
    UnknownKineticsModel(const std::string& proc, const std::string& kineticsModel) :
        CanteraError(proc, "Specified Kinetics model "
                     + kineticsModel +
                     " does not match any known type.") {}
};

/**
 * Factory for kinetics managers.
 */
class KineticsFactory : public Factory<Kinetics>
{
public:
    static KineticsFactory* factory() {
        std::unique_lock<std::mutex> lock(kinetics_mutex);
        if (!s_factory) {
            s_factory = new KineticsFactory;
        }
        return s_factory;
    }

    virtual void deleteFactory() {
        std::unique_lock<std::mutex> lock(kinetics_mutex);
        delete s_factory;
        s_factory = 0;
    }

    /**
     * Return a new kinetics manager that implements a reaction mechanism
     * specified in a CTML file. In other words, the kinetics manager, given
     * the rate constants and formulation of the reactions that make up a
     * kinetics mechanism, is responsible for calculating the rates of
     * progress of the reactions and for calculating the source terms for
     * species.
     *
     * @param phase An XML_Node that contains the XML data describing the
     *              phase. Of particular note to this routine is the child XML
     *              element called "kinetics". The element has one attribute
     *              called "model", with a string value. The value of this
     *              string is used to decide which kinetics manager is used to
     *              calculate the reaction mechanism.
     * @param th    Vector of phases. The first phase is the phase in which
     *              the reactions occur, and the subsequent phases (if any)
     *              are e.g. bulk phases adjacent to a reacting surface.
     * @return Pointer to the new kinetics manager.
     */
    virtual Kinetics* newKinetics(XML_Node& phase, std::vector<ThermoPhase*> th);

    /**
     * Return a new, empty kinetics manager.
     */
    virtual Kinetics* newKinetics(const std::string& model);

private:
    static KineticsFactory* s_factory;
    KineticsFactory();
    static std::mutex kinetics_mutex;
};

/**
 *  Create a new kinetics manager.
 *  @deprecated The `KineticsFactory*` argument to this function is deprecated
 *     and will be removed after Cantera 2.3.
 */
inline Kinetics* newKineticsMgr(XML_Node& phase,
                                std::vector<ThermoPhase*> th, KineticsFactory* f=0)
{
    if (f == 0) {
        f = KineticsFactory::factory();
    } else {
        warn_deprecated("newKineticsMgr(XML_Node&, KineticsFactory*)",
            "The `KineticsFactory*` argument to this function is deprecated and"
            " will be removed after Cantera 2.3.");
    }
    return f->newKinetics(phase, th);
}

/**
 *  Create a new kinetics manager.
 *  @deprecated The `KineticsFactory*` argument to this function is deprecated
 *     and will be removed after Cantera 2.3.
 */
inline Kinetics* newKineticsMgr(const std::string& model, KineticsFactory* f=0)
{
    if (f == 0) {
        f = KineticsFactory::factory();
    } else {
        warn_deprecated("newKineticsMgr(string, KineticsFactory*)",
            "The `KineticsFactory*` argument to this function is deprecated and"
            " will be removed after Cantera 2.3.");
    }
    return f->newKinetics(model);
}
}

#endif
