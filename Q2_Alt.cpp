#include <cmath>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <chrono> //For timing the code.
#include <iostream>

#define ALIVE 0x01
#define SICK 0x02
#define IMMUNE 0x04

const int initial = 100;
const int maxPop = 200000;
const int infectiousness = 50;
const int deathRate = 1;
const int recoveryRate = 12;

unsigned int fate_tester(int odds) {
    return static_cast<unsigned int>(ceil(static_cast<double>(rand()) / (static_cast<long>(RAND_MAX)) * odds));
}

class person {
public:
    person(bool);

    void sicken();

    void kill();

    void immunise();

    bool sick();

    bool alive();

    bool immune();

    void stats();
    u_char p{0};
};

person::person(bool sick) {
    if (sick) {
        p |= 1UL << SICK;
    }
    p |= 1UL << ALIVE;
};

void person::sicken() {
    p |= 1UL << SICK;
}

void person::kill() {
    p &= ~(1UL << ALIVE);
    p &= ~(1UL << SICK);
}

void person::immunise() {
    p &= ~(1UL << SICK);
    p |= 1UL << IMMUNE;
}


void person::stats() {
    std::cout << "Sick? " << this->sick() << std::endl;
    std::cout << "Alive? " << this->alive() << std::endl;
    std::cout << "Immune? " << this->immune() << std::endl;
    std::cout << "\n\n" << std::endl;
}
bool person::sick() {
    bool bit = static_cast<bool>((p >> SICK) & 1U);
    return bit;
}

bool person::alive() {
    bool bit = static_cast<bool>((p >> ALIVE) & 1U);
    return bit;
}

bool person::immune() {
    bool bit = static_cast<bool>((p >> IMMUNE) & 1U);
    return bit;
}

class population {
public:
    population();

    person *peoples[maxPop];
};

population::population() {
    for (int i = 0; i <= maxPop; i++) {
        peoples[i] = new person(i <= initial);
    }
}


int main() {
    auto pop = new population;
    std::ofstream brains;
    brains.open("Pandemic.csv");
    brains << "Day,Infected,Uninfected,Immune,Alive,Dead\r";
//    auto person =pop->peoples[101];
//    std::cout << "Sick? " << person->sick() << std::endl;
//    std::cout << "Alive? " << person->alive() << std::endl;
//    std::cout << "Immune? " << person->immune() << std::endl;
//
//    person->sicken();
//
//    std::cout << "\n\nSick? " << person->sick() << std::endl;
//    std::cout << "Alive? " << person->alive() << std::endl;
//    std::cout << "Immune? " << person->immune() << std::endl;
//
//    person->immunise();
//
//    std::cout << "\n\nSick? " << person->sick() << std::endl;
//    std::cout << "Alive? " << person->alive() << std::endl;
//    std::cout << "Immune? " << person->immune() << std::endl;
    unsigned int deadCount = 0;
    unsigned int recoveredCount = 0;
    float scaledMeetingsPerDay = (50000/200000)*200000;
    for (int i = 0; i <= 365; i++) {
        unsigned int sickCount = 0;
//        std::cout << scaledMeetingsPerDay << std::endl;
        for (int j = 0; j <= scaledMeetingsPerDay; j++) {
            int personOneSeed = fate_tester(maxPop);
            int personTwoSeed = fate_tester(maxPop);
            if (personOneSeed == personTwoSeed) {
                personOneSeed = fate_tester(maxPop);
//                continue;
            }
            auto person1 = pop->peoples[personOneSeed];
            auto person2 = pop->peoples[personTwoSeed];

            if ((person1->alive() && person2->alive()) && (!person1->immune() && !person2->immune())) {
                if (person1->sick() != person2->sick()) {
                    if (fate_tester(100) <= infectiousness) {
//                        person1->stats();
//                        person2->stats();
                        if (!person1->sick()) {
                            person1->sicken();
                        } else {
                            person2->sicken();
                        }
//                        person1->stats();
//                        person2->stats();
//                        std::cout << "\n\n\n\n" << std::endl;
                    }
                }
            }
        }

        for (int k = 0; k <= maxPop; k++) {
            auto person = pop->peoples[k];
            if (person->sick() && person->alive()) {
                if (fate_tester(100) <= recoveryRate) {
                    person->immunise();
                    recoveredCount++;
                } else if (fate_tester(100) <= deathRate) {
                    person->kill();
                    deadCount++;
                }
                else{
                    sickCount++;
                }
            }
        }
        unsigned int aliveCount = maxPop - deadCount;
        int uninfectedCount = maxPop - sickCount - deadCount - recoveredCount;
        scaledMeetingsPerDay = (50000.0/200000)*aliveCount;
        brains << i << "," << sickCount << "," << uninfectedCount << "," << recoveredCount << "," << aliveCount << ","
               << deadCount << "," << "\r" << std::endl;
    }
}
