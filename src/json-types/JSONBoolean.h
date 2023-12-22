#ifndef JSON_BOOLEAN
#define JSON_BOOLEAN

class JSONBoolean {
   private:
    bool m_value;

   public:
    JSONBoolean(bool value) : m_value(value) {}
    bool value();

    void print(uint64_t indent);
};

#endif