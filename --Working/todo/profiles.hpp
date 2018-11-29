#ifndef _PROFILES_HPP_
#define _PROFILES_HPP_

#include "main.h"

typedef void (*profileFunction)(void);

/*
 * Class meant to hold and manage driver profiles
 */

class Profiles {
  private:
    static int selectedProfile;
    static std::vector<std::pair<std::string, profileFunction>> profiles;

  public:
    // Flag to control whether the current profile should continue running
    static volatile bool unchangedProfile;

    // An empty profile
    static void empty();

    // Registers profiles
    static void registerAll();

    // Registers a profile, returning the registered profile's index
    static int registerProfile(std::string name, profileFunction function);

    // Runs the selected profile
    static void run();

    // Selects the next profile
    static void next();

    // Selects the previous profile
    static void prev();

    // Selects the profile of the given index
    static void select(int index);

    // Returns the selected profile name
    static std::string getSelectedName();

    // Returns the profile name of the given index
    static std::string getProfileName(int index);

};

#endif
