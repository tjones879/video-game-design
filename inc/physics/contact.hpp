#include <vector>

namespace phy {
class Contact {

};

class ContactFinder {
    std::vector<Contact> contacts;
public:
    ContactFinder();
    void FindAllContacts();
    void destroyContact();
    void collide();
};
} /* namespace phy */
