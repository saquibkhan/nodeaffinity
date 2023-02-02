const axios = require("axios");
const XLSX = require("xlsx");
const token = process.env.TOKEN;
const headers = {
  "Authorization": `Token ${token}`
};
const repoName = "nodeaffinity";
const repoOwner = "saquibkhan";
axios.get(`https://api.github.com/repos/${repoOwner}/${repoName}/issues`, { headers })
 .then(response => {
  const workbook = XLSX.utils.json_to_sheet(response.data);
  XLSX.writeFile(XLSX.utils.book_new([workbook]), "issues.xlsx");
