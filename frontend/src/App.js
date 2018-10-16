import React, { Component } from "react";
import axios from "axios";

import EOSIOClient from "./utils/eosio-client";
import IOClient from "./utils/io-client";
import { updateInvoicesForCreateAndEdit } from "./utils/invoices-updater";

class App extends Component {
  // Instantiate shared eosjs helper and socket io helper
  constructor(props) {
    super(props);
    const contractAccount = process.env.REACT_APP_EOSIO_CONTRACT_ACCOUNT;
    this.eosio = new EOSIOClient(contractAccount);
    this.io = new IOClient();
    this.state = {
      invoices: []
    };
  }

  // Enable Realtime updates via Socket.io
  async componentDidMount() {
    this.loadInvoices();
    this.io.onMessage("initiate", invoice => {
      this.setState(prevState => ({
        invoices: updateInvoicesForCreateAndEdit(prevState, invoice)
      }));
    });
  }

  // Create a invoice
  createInvoice = async invoice => {
    try {
      const newInvoice = {
        ...invoice,
        _id: {
          timestamp: Math.floor(Date.now() / 1000),
          author: process.env.REACT_APP_EOSIO_ACCOUNT
        },
        author: process.env.REACT_APP_EOSIO_ACCOUNT
      };

      await this.eosio.transaction(
        process.env.REACT_APP_EOSIO_ACCOUNT,
        "initiate",
        {
          timestamp: newInvoice._id.timestamp,
          author: newInvoice._id.author,
          ...invoice
        }
      );

      this.setState(prevState => ({
        invoices: updateInvoicesForCreateAndEdit(prevState, newInvoice)
      }));
    } catch (err) {
      console.error(err);
    }
  };

  // Load invoices
  loadInvoices = async () => {
    const response = await axios.get(
      `${process.env.REACT_APP_API_URL}/invoices`
    );
    this.setState({ invoices: response.data.reverse() });
  };

  render() {
    return <div className="App">s</div>;
  }
}

export default App;
