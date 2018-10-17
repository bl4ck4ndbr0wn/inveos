
#include <eosiolib/eosio.hpp>

using namespace eosio;
using namespace std;

class supplychain : public contract {
  public:
    supplychain(account_name self):contract(self), _orders(self, self), _approvals(self, self){}

  [[eosio::action]]
  void order(account_name purchaser, account_name supplier, account_name fnc, uint64_t id){
    require_auth(purchaser);

    // Save to table
    eosio_assert( _orders.find( id ) == _orders.end(), "An order with this ID exist" );

      
         _orders.emplace( purchaser, [&]( auto& rcrd ) {
            rcrd.id = id;
            rcrd.purchaser    = purchaser;
            rcrd.supplier    = supplier;
            rcrd.fnc = fnc;
      
         });
        //  pay(purchaser, supplier, "10.0000 SYS");
  }

  [[eosio::action]]
  void approve(uint64_t orderId, account_name approver, string approver_category){
    require_auth(approver);

    _approvals.emplace(orderId, [&](auto& rcrd){
      rcrd.orderId = orderId;
      rcrd.approver = approver;
      if(approver_category.compare("SFC") == 0){
        rcrd.supplier_approval = "Approved";
      }
      else if(approver_category == "Fnc"){
        rcrd.financier_approval = "Approved";
      }
    });


  }


  [[eosio::action]]
  void acknowledge(uint64_t orderId, account_name fnc, account_name supplier){
    require_auth(fnc);



  }
  private:

    struct [[eosio::table]] ordert{
      uint64_t id;
      account_name purchaser;
      account_name supplier;
      account_name fnc;

      uint64_t primary_key() const {return id;}
    };
    typedef eosio::multi_index<N(orders), ordert> orders_table;

    struct [[eosio::table]] approval{
      uint64_t id;
      uint64_t orderId;
      account_name approver;
      string supplier_approval;
      string financier_approval;

      uint64_t primary_key() const {return id;}
    };
  typedef eosio::multi_index<N(approvals), approval> approvals_table;

  orders_table _orders;

  approvals_table _approvals;
  void pay(account_name from, account_name to, string quantity){
        action(
        permission_level{ from, N(active) },
        N(eosio.token), N(transfer),
        std::make_tuple(from, to, quantity, std::string(""))
     ).send();
  }


};

EOSIO_ABI(supplychain, (order)(approve))
