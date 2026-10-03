from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session
from datetime import datetime

from ..database import SessionLocal
from ..models import Reading, Sensor
from ..schemas import ReadingOut, ReadingCreate

router = APIRouter()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@router.post("/readings", response_model=ReadingOut)
def create_reading(reading: ReadingCreate, db: Session = Depends(get_db)):
    sensor = db.query(Sensor).filter(Sensor.id == reading.sensor_id).first()
    if not sensor:
        raise HTTPException(status_code=404, detail="Sensor >>")

    db_reading = Reading(sensor_id=reading.sensor_id, value=reading.value)
    db.add(db_reading)
    db.commit()
    db.refresh(db_reading)

    return db_reading

#Не отклоняй пжпжпжпж