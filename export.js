const axios = require("axios");
const XLSX = require("xlsx");
const token = process.env.GITHUB_TOKEN;
const headers = {
  "Authorization": `Token ${token}`
};
const repoName = "nodeaffinity";
const repoOwner = "saquibkhan";
axios.get(`https://api.github.com/repos/${repoOwner}/${repoName}/issues`, { headers })
 .then(response => {
 console.log("************** response data *******************");
  console.log(response.data);
  
  const filteredData = response.data.filter(obj => obj.issueName && obj.id && obj.createdDate && obj.closedDate && obj.labels);

const desiredData = filteredData.map(obj => ({
  "Issue Name": obj.title,
  "ID": obj.id,
  "Created Date": obj.created_at,
  "Closed Date": obj.closed_at,
  "Labels": obj.labels
}));

 console.log("************** desiredData *******************");
  console.log(desiredData);
  
const workbook = XLSX.utils.json_to_sheet(desiredData);
  
  
  //const workbook = XLSX.utils.json_to_sheet(desiredData);
   console.log("************** workbook *******************");
  console.log(workbook);
XLSX.writeFile(XLSX.utils.book_new([workbook]), "issues.xlsx");});
