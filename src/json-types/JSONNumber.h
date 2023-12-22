#ifndef JSON_NUMBER
#define JSON_NUMBER

class JSONNumber {
   private:
    double m_value;

   public:
    JSONNumber(double value) : m_value(value) {}
    double value();

    void print(uint64_t indent);
};

#endif