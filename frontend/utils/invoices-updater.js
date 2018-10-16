// Update the edited invoice od add invoice to invoices if new invoice
export const updateInvoicesForCreateAndEdit = (prevState, updatedInvoice) => {
  let alreadyAdded = false;
  let updatedInvoices = prevState.invoices.map(invoice => {
    if (invoice._id === updatedInvoice.invoice_id) {
      alreadyAdded = true;
      return {
        ...invoice,
        ...updatedInvoice
      };
    }
    return invoice;
  });

  if (!alreadyAdded) {
    updatedInvoices = [{ ...updatedInvoice }, ...updatedInvoices];
  }

  return updatedInvoices;
};
