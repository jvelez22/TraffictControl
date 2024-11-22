import React, { useEffect, useState } from "react";
import { collection, query, orderBy, onSnapshot } from "firebase/firestore";
import db from "../firebase";
import styled from "styled-components";

// Styled components
const DashboardContainer = styled.div`
  padding: 20px;
  max-width: 1200px;
  margin: 0 auto;
  font-family: Arial, sans-serif;
`;

const Title = styled.h1`
  text-align: center;
  color: #333;
  margin-bottom: 20px;
`;

const Table = styled.table`
  width: 100%;
  border-collapse: collapse;
  margin-top: 20px;
`;

const TableHead = styled.thead`
  background-color: #4caf50;
  color: white;
`;

const TableHeader = styled.th`
  padding: 10px;
  text-align: left;
  border: 1px solid #ddd;
`;

const TableRow = styled.tr`
  &:nth-child(even) {
    background-color: #f2f2f2;
  }

  &:hover {
    background-color: #ddd;
  }
`;

const TableCell = styled.td`
  padding: 10px;
  border: 1px solid #ddd;
`;

const NoDataMessage = styled.div`
  text-align: center;
  margin-top: 20px;
  font-size: 18px;
  color: #777;
`;

const AveragesRow = styled.tr`
  background-color: #e0f7fa;
  font-weight: bold;
`;

const Dashboard = () => {
  const [data, setData] = useState([]);
  const [averages, setAverages] = useState({ CO2: 0, Time1: 0, Time2: 0 });

  useEffect(() => {
    // Query Firestore collection and order by createdAt descending
    const q = query(collection(db, "data"), orderBy("createdAt", "desc"));

    const unsubscribe = onSnapshot(q, (snapshot) => {
      const newData = snapshot.docs.map((doc) => ({
        id: doc.id,
        ...doc.data(),
      }));
      setData(newData);

      // Calculate averages
      if (newData.length > 0) {
        const totalCO2 = newData.reduce((sum, item) => sum + (item.CO2 || 0), 0);
        const totalTime1 = newData.reduce((sum, item) => sum + (item.Time1 || 0), 0);
        const totalTime2 = newData.reduce((sum, item) => sum + (item.Time2 || 0), 0);

        setAverages({
          CO2: (totalCO2 / newData.length).toFixed(2),
          Time1: (totalTime1 / newData.length).toFixed(2),
          Time2: (totalTime2 / newData.length).toFixed(2),
        });
      } else {
        setAverages({ CO2: 0, Time1: 0, Time2: 0 });
      }
    });

    return () => unsubscribe(); // Cleanup listener on unmount
  }, []);

  return (
    <DashboardContainer>
      <Title>Firestore Dashboard</Title>
      {data.length > 0 ? (
        <Table>
          <TableHead>
            <tr>
              <TableHeader>ID</TableHeader>
              <TableHeader>CO2</TableHeader>
              <TableHeader>Time1</TableHeader>
              <TableHeader>Time2</TableHeader>
              <TableHeader>Created At</TableHeader>
            </tr>
          </TableHead>
          <tbody>
            {data.map((item) => (
              <TableRow key={item.id}>
                <TableCell>{item.id}</TableCell>
                <TableCell>{item.CO2}</TableCell>
                <TableCell>{item.Time1}</TableCell>
                <TableCell>{item.Time2}</TableCell>
                <TableCell>
                  {item.createdAt
                    ? new Date(item.createdAt.seconds * 1000).toLocaleString()
                    : "N/A"}
                </TableCell>
              </TableRow>
            ))}
            {/* Add Averages Row */}
            <AveragesRow>
              <TableCell>Average</TableCell>
              <TableCell>{averages.CO2}</TableCell>
              <TableCell>{averages.Time1}</TableCell>
              <TableCell>{averages.Time2}</TableCell>
              <TableCell></TableCell>
            </AveragesRow>
          </tbody>
        </Table>
      ) : (
        <NoDataMessage>No data available</NoDataMessage>
      )}
    </DashboardContainer>
  );
};

export default Dashboard;
