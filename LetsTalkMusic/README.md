# LetsTalkMusic

> Letterboxd, but for music.

LetsTalkMusic (the React client is codenamed **Retunify**) is a full-stack web app for logging, rating, and reviewing albums. Browse artists, dive into their discographies, read the tracklist for any album, and leave a 0–5 star rating with a short written review. Every user gets a profile page that collects their reviews.

## Features

- **Accounts:** sign up and log in. Passwords are hashed with bcrypt and must be at least 12 characters.
- **Browse artists:** a homepage listing artists with their genres and artwork.
- **Discographies:** view every album by an artist.
- **Album pages:** synopsis, full tracklist with track lengths, and all community reviews.
- **Ratings and reviews:** rate albums from 0 to 5 and write a review of up to 255 characters. Reviews can be created, edited, and deleted.
- **Profile page:** see and manage all of your own reviews.
- **Admin dashboard:** a Flask-Admin panel for managing users, artists, albums, tracks, and reviews, protected by an admin key.
- **Seed data:** a starter catalog of 23 artists and 40 albums, loaded with a single script.

## Tech Stack

| Layer | Technology |
| --- | --- |
| Frontend | React 19, Vite, React Router, Material UI (MUI), React Compiler |
| Backend | Python, Flask, Flask-SQLAlchemy, Flask-CORS, Flask-Admin |
| Database | SQLite (`server/MusicApp.db`, created automatically) |
| Auth | bcrypt password hashing |
| Deployment | Gunicorn via `Procfile` (Heroku-style) |

## Project Structure

```
LetsTalkMusic/
├── client/
│   └── Retunify/               # React + Vite frontend
│       ├── public/
│       └── src/
│           ├── pages/          # Login, CreateAccount, Homepage, ArtistAlbums,
│           │                   #   TracksAndReviews, Profilepage
│           ├── components/     # ReviewItem, WeeklyDigest
│           ├── Test_Pages/     # Dev-only pages for testing API calls
│           └── assets/         # Album covers, artist images, backgrounds
└── server/
    ├── app.py                  # Flask app, DB models, API routes, admin views
    ├── populate.py             # Seeds the database from seed_data.json
    ├── seed_data.json          # Starter artists, albums, and tracks
    ├── example.md              # Template for adding a new artist to the seed data
    ├── requirements.txt
    └── Procfile                # gunicorn entry point for deployment
```

## Getting Started

### Prerequisites

- [Node.js](https://nodejs.org/) (a current LTS release) and npm
- [Python 3](https://www.python.org/downloads/) and pip

### 1. Clone the repository

```bash
git clone https://github.com/Bryan-Soda/LetsTalkMusic.git
cd LetsTalkMusic
```

### 2. Set up the backend

```bash
cd server
python -m venv .venv
source .venv/bin/activate        # Windows: .venv\Scripts\activate
pip install -r requirements.txt
```

Create a `.env` file in `server/`:

```env
FLASK_ADMIN_KEY=choose-a-long-random-string
FRONTEND_URL=http://localhost:5173
PORT=5000
```

| Variable | Purpose | Default |
| --- | --- | --- |
| `FLASK_ADMIN_KEY` | Secret used for the Flask `SECRET_KEY` and to unlock the admin dashboard | none (required) |
| `FRONTEND_URL` | Origin allowed by CORS | `http://localhost:5173` |
| `PORT` | Port the server listens on | `5000` |

Seed the database, then start the server:

```bash
python populate.py
python app.py
```

The API is now running at `http://localhost:5000`.

### 3. Set up the frontend

In a second terminal:

```bash
cd client/Retunify
npm install
```

Create a `.env` file in `client/Retunify/` pointing at your backend:

```env
VITE_API_URL=http://localhost:5000
```

Start the dev server:

```bash
npm run dev
```

Open the URL Vite prints (usually `http://localhost:5173`).

### Frontend scripts

| Command | Description |
| --- | --- |
| `npm run dev` | Start the Vite dev server with hot reload |
| `npm run build` | Build for production |
| `npm run preview` | Preview the production build locally |
| `npm run lint` | Run ESLint |

## API Reference

| Method | Endpoint | Description |
| --- | --- | --- |
| `POST` | `/user` | Create an account (`username`, `password`) |
| `POST` | `/auth` | Log in (`username`, `password`) |
| `GET` | `/artists` | List all artists |
| `GET` | `/artists/<artist_id>` | Get one artist |
| `GET` | `/artists/<artist_id>/albums` | List an artist's albums |
| `GET` | `/artists/<artist_id>/<album_id>` | Album details and tracklist |
| `GET` | `/reviews/album/<album_id>` | All reviews for an album |
| `GET` | `/reviews/user/<user_id>` | All reviews by a user |
| `GET` | `/reviews/<user_id>/<album_id>` | A user's review of an album |
| `POST` | `/reviews/<user_id>/<album_id>` | Create a review (`rating` 0–5, `review` up to 255 chars) |
| `PUT` | `/reviews/<user_id>/<album_id>` | Edit a review |
| `DELETE` | `/reviews/<user_id>/<album_id>` | Delete a review |

## Frontend Routes

| Path | Page |
| --- | --- |
| `/` | Login |
| `/create-account` | Account creation |
| `/home` | Homepage / artist browser |
| `/artists/:artistId` | An artist's albums |
| `/album/:artistId/:albumId` | Album tracks and reviews |
| `/profile/` | Your profile and reviews |

## Admin Dashboard

Flask-Admin is mounted at `/admin`. Access is gated by the `FLASK_ADMIN_KEY` value, which is passed as a query parameter:

```
http://localhost:5000/admin/?admin_key=<your FLASK_ADMIN_KEY>
```

Keep this key secret and never commit your `.env` file (it is already listed in `.gitignore`).

## Adding Music to the Catalog

Artists, albums, and tracks live in `server/seed_data.json`. Use the structure in `server/example.md` as a template:

```json
{
    "artist_name": "NAME",
    "genre": "GENRE",
    "albums": [
        {
            "title": "TITLE",
            "total_length": "00:00",
            "synopsis": "A short description of the album.",
            "tracks": [
                { "title": "TITLE", "length": "0:00" }
            ]
        }
    ]
}
```

Then run `python populate.py` again. Existing artists, albums, and tracks are skipped, so it's safe to re-run. Artist and album artwork are bundled in `client/Retunify/src/assets/` and need to be added there as well.

## Deployment

- **Backend:** the `Procfile` runs `gunicorn app:app --bind 0.0.0.0:$PORT`. Set `FLASK_ADMIN_KEY` and `FRONTEND_URL` (your deployed frontend's origin) in your host's environment settings.
- **Frontend:** run `npm run build` and deploy the `dist/` folder to any static host. Set `VITE_API_URL` to your deployed backend URL. `client/Retunify/.env.production` currently holds a placeholder value to replace.

**Link:** https://github.com/Bryan-Soda/LetsTalkMusic.git
