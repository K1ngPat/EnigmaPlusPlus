#include <iostream>
#include <vector>
using namespace std;

// The rotor class represents a single rotor in the Enigma machine.
class Rotor {
public:
    Rotor(const vector<int>& wiring, int start_pos) : position(start_pos), notch(0) {
        for (int i = 0; i < 26; ++i) {
            mapping.push_back(wiring[(i + start_pos) % 26]);
        }
    }

    // Turn the rotor by one position.
    void advance() {
        position = (position + 1) % 26;
    }

    // Set the notch position for this rotor.
    void set_notch(int notch_pos) { // TODO: Add explanation of how this is merely a question of physical examination of rotor wheels
        notch = notch_pos%26;
    }

    // Get the output for a given input, taking into account the current rotor position.
    int encrypt(int input) const {
        int offset = (input + position) % 26;
        int output = mapping[offset];
        int shifted_output = (output - position+26) % 26;
        return shifted_output;
    }

    int dencrypt(int shifted_input) const {
        int input = (shifted_input + position)%26;
        int i;
        for(i=0; i<26; i++)
        {
            if(mapping[i] == input)
            {
                break;
            }
        }
        int output = (i-position+26)%26;
        return output;

    }

    // Check if this rotor is in a notch position.
    bool is_at_notch() const {
        return (position == notch);
    }


    vector<int> mapping;
    int position;
    int notch;
};

// The reflector class represents the fixed reflector in the Enigma machine.
class Reflector {
public:
    Reflector(const vector<int>& mapping) {
        for (int i = 0; i < 26; ++i) {
            this->mapping.push_back(mapping[i]);
        }
    }

    // Get the output for a given input.
    int reflect(int input) const {
        return mapping[input];
    }

private:
    vector<int> mapping;
};

// The Enigma class represents the full Enigma machine.
class Enigma {
public:
    Enigma(const vector<Rotor>& rotors, const Reflector& reflector) : rotors(rotors), reflector(reflector), rotate_left(false) {
    }

    // Set the initial rotor positions for the machine.
    void set_rotor_positions(const vector<int>& positions) {
        for (int i = 0; i < positions.size(); ++i) {
            vector<int> a;
            for(int j=0; j<26; j++)
            {
                a.push_back(rotors[i].mapping[j]);
            }
            rotors[i] = Rotor(a, positions[i]);
        }
    }

    // Encrypt a single character.
    int encrypt(int input) {
        // Rotate the rotors before encryption.
        rotate_rotors();

        // Pass the input through the rotors.
        int output = input;
        for (int i = rotors.size() - 1; i >= 0; --i) {
            output = rotors[i].encrypt(output);
        }

        // Pass the output through the reflector.
        output = reflector.reflect(output);

        // Pass the output back through the rotors in reverse order.
        for (int i = 0; i < rotors.size(); ++i) {
            output = rotors[i].dencrypt(output);
        }

        // Rotate the rightmost rotor.
        rotors[rotors.size() - 1].advance();

        // Return the encrypted character.
        return output;
    }

    int* encrypt_string(int A[], int len)
    {
        int *B = new int[len];
        for(int i=0; i<len; i++)
        {
            B[i] = encrypt(A[i]);
        }

        return B;
    }

private:
    vector<Rotor> rotors;
    Reflector reflector;
    bool rotate_middle;
    bool rotate_left;

    // Rotate the rotors as needed.
    void rotate_rotors() {
        rotate_middle = (rotors[2].is_at_notch() || rotate_left);
        rotate_left = rotors[1].is_at_notch();

        if (rotate_left && rotate_middle) {
            rotors[0].advance();
            rotors[1].advance();
        }
        else if(rotate_middle){
            rotors[1].advance();
        }

    }


};

int* Plugboard(int plugs[], int num_plugs, int content[], int len)
{

}


int main()
{
    vector<int> rotor1map{2, 4, 6, 8, 10, 12, 3, 16, 18, 20, 24, 22, 14, 25, 5, 9, 23, 7, 1, 11, 13, 21, 19, 17, 15, 0};
    vector<int> rotor2map{14, 19, 4, 15, 20, 17, 8, 23, 18, 16, 24, 0, 22, 3, 9, 2, 13, 12, 21, 7, 25, 11, 1, 10, 6, 5};
    vector<int> rotor3map{5, 21, 15, 7, 19, 14, 23, 6, 25, 2, 17, 1, 11, 24, 10, 12, 9, 16, 20, 18, 0, 8, 22, 3, 13, 4};
    vector<int> reflectormap{17, 20, 22, 11, 24, 19, 15, 16, 13, 10, 9, 3, 18, 8, 21, 6, 7, 0, 12, 5, 1, 14, 2, 25, 4, 23};
    Rotor rtr1(rotor1map, 2);
    Rotor rtr2(rotor2map, 17);
    Rotor rtr3(rotor3map, 3);
    vector<Rotor> rtrs{rtr1, rtr2, rtr3};
    Reflector rfr(reflectormap);
    Enigma enigma1(rtrs, rfr);

    
    char text[700];
    cin.getline(text, 700);
    int toenc[700];
    int ln = 0;
    
    while(text[ln] != 0)
    {
        toenc[ln] = int(text[ln] - 'A');
        ln++;

    }
    int* fromenc = enigma1.encrypt_string(toenc, ln);
    cout<<"\n\n";

    for(int i = 0; i<ln; i++)
    {
        cout<<char(fromenc[i] + 65);
    }



    return 0;
}