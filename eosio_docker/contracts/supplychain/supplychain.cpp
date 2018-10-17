
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

      
         _orders.emplace( id, [&]( auto& rcrd ) {
            rcrd.id = id;
            rcrd.purchaser    = purchaser;
            rcrd.supplier    = supplier;
            rcrd.fnc = fnc;
      
         });
  }

  [[eosio::action]]
  void approve(uint64_t orderId, account_name approver, string approver_category){
    require_auth(approver);

    _approvals.emplace(id, [&](auto& rcrd){
      rcrd.id = id;
      rcrd.approver = approver;
      if(approver_category == 'Supplier'){
        rcrd.supplier_approval = 'Approved';
      }
      else if(approver_category == 'Fnc'){
        rcrd.financier_approval = 'Approved';
      }
    });


  }
  private:

    struct [[eosio::table]] order{
      uint64_t id;
      account_name purchaser;
      account_name supplier;
      account_name fnc;

      uint64_t primary_key() const {return id;}
    }
    typedef eosio::multi_index<N(orders), order> orders_table;

    struct [[eosio::table]] approval{
      uint64_t id;
      uint64_t orderId;
      account_name approver;
      string supplier_approval;
      string financier_approval;

      uint64_t primary_key() const {return id;}
    }
  typedef eosio::multi_index<N(approvals), approval> approvals_table;

  orders_table _orders;

  approvals_table _approvals;

};

EOSIO_ABI(supplychain, (order)(approve))
