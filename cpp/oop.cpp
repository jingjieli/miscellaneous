// Object-oriented Programming 
// Key: data abstraction, inheritance, dynamic binding

class Quote {
public:
	Quote() = default;
	Quote(const std::string &book, double sales_price)
	: bookNo(book), price(sales_price) { };
	std::string isbn() const { return bookNo; };
	virtual double net_price(std::size_t n) const { return n*price };
	virtual ~Quote() = default;
private:
	std::string bookNo;
protected:
	double price = 0.0;
}

class BulkQuote : public Quote {
public:
	BulkQuote() = default;
	BulkQuote(const std::string& book, double p, std::size_t qty, double disc) 
	: Quote(book, p), min_qty(qty), discount(disc) { };
	double net_price(std::size_t n) const override;
private:
	std::size_t min_qty = 0;
	double discount = 0.0;
}

double BulkQuote::net_price(std::size_t cnt) const {
	if (cnt >= min_qty) {
		return cnt * (1 - discount) * price;
	} else {
		return cnt * price;
	}
}

// What is inherited from base class ? 
// In principle, a publicly derived class inherits access to every member 
// of a base class except:
// * Its constructor and deconstructor
// * Its assignment operator members (operator=)
// * Its friends
// * Its private members

// Even though construcotr and deconstructor are not inherited, they're 
// automatically called by the constructor and deconstrutor of derived class
// Unless otherwise specified, the constructor of a derived class calls the 
// default constructor of its base class (i.e. the one takes no arguments)
