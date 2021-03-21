function searchIds() {
    var input, filter, tbody, td, a, i, txtValue;
    input = document.getElementById('input-search-id');
    filter = input.value.toUpperCase();
    tbody = document.getElementById("ids-list");
    td = tbody.getElementsByTagName('td');
    for (i = 0; i < td.length; i++) {
      a = td[i].getElementsByTagName("a")[0];
      txtValue = a.textContent || a.innerText;
      if (txtValue.toUpperCase().indexOf(filter) > -1) {
        td[i].parentElement.style.display = "";
      } else {
        td[i].parentElement.style.display = "none";
      }
    }
  }